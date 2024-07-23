//* Copyright 2017 The Dawn & Tint Authors
//*
//* Redistribution and use in source and binary forms, with or without
//* modification, are permitted provided that the following conditions are met:
//*
//* 1. Redistributions of source code must retain the above copyright notice, this
//*    list of conditions and the following disclaimer.
//*
//* 2. Redistributions in binary form must reproduce the above copyright notice,
//*    this list of conditions and the following disclaimer in the documentation
//*    and/or other materials provided with the distribution.
//*
//* 3. Neither the name of the copyright holder nor the names of its
//*    contributors may be used to endorse or promote products derived from
//*    this software without specific prior written permission.
//*
//* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "dawn/wire/WireCmd_autogen.h"

#include "dawn/common/Assert.h"
#include "dawn/common/Log.h"
#include "dawn/common/Numeric.h"
#include "dawn/wire/BufferConsumer_impl.h"
#include "dawn/wire/Wire.h"

#include <algorithm>
#include <cstring>
#include <limits>

#if defined(__GNUC__) || defined(__clang__)
// error: 'offsetof' within non-standard-layout type 'wgpu::XXX' is conditionally-supported
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif

//* Helper macros so that the main [de]serialization functions can be written in a generic manner.

//* Outputs an rvalue that's the number of elements a pointer member points to.
{%- macro member_length(member, record_accessor) -%}
    {%- if member.length == "constant" -%}
        {{member.constant_length}}u
    {%- else -%}
        {{record_accessor}}{{as_varName(member.length.name)}}
    {%- endif -%}
{%- endmacro -%}

//* Outputs the type that will be used on the wire for the member
{%- macro member_transfer_type(member) -%}
    {%- if member.type.category == "object" -%}
        ObjectId
    {%- elif member.type.category == "structure" -%}
        {{as_cType(member.type.name)}}Transfer
    {%- elif as_cType(member.type.name) == "size_t" -%}
        {{as_cType(types["uint64_t"].name)}}
    {%- else -%}
        {%- do assert(member.type.is_wire_transparent, 'wire transparent') -%}
        {{as_cType(member.type.name)}}
    {%- endif -%}
{%- endmacro -%}

//* Outputs the size of one element of the type that will be used on the wire for the member
{%- macro member_transfer_sizeof(member) -%}
    sizeof({{member_transfer_type(member)}})
{%- endmacro -%}

//* Outputs the serialization code to put `in` in `out`
{%- macro serialize_member(member, in, out) -%}
    {%- if member.type.category == "object" -%}
        {%- set Optional = "Optional" if member.optional else "" -%}
        WIRE_TRY(provider.Get{{Optional}}Id({{in}}, &{{out}}));
    {%- elif member.type.category == "structure" -%}
        //* Do not memcpy or we may serialize padding bytes which can leak information across a
        //* trusted boundary.
        {%- set Provider = ", provider" if member.type.may_have_dawn_object else "" -%}
        WIRE_TRY({{as_cType(member.type.name)}}Serialize({{in}}, &{{out}}, buffer{{Provider}}));
    {%- elif not is_wire_serializable(member.type) -%}
        if ({{in}} != nullptr) return WireResult::FatalError;
    {%- else -%}
        {{out}} = {{in}};
    {%- endif -%}
{%- endmacro -%}

//* Outputs the deserialization code to put `in` in `out`
{%- macro deserialize_member(member, in, out) -%}
    {%- if member.type.category == "object" -%}
        {%- set Optional = "Optional" if member.optional else "" -%}
        WIRE_TRY(resolver.Get{{Optional}}FromId({{in}}, &{{out}}));
    {%- elif member.type.category == "structure" %}
        {% if member.type.is_wire_transparent %}
            static_assert(sizeof({{out}}) == sizeof({{in}}), "Deserialize memcpy size must match.");
                memcpy(&{{out}}, const_cast<const {{member_transfer_type(member)}}*>(&{{in}}), {{member_transfer_sizeof(member)}});
        {%- else %}
            WIRE_TRY({{as_cType(member.type.name)}}Deserialize(&{{out}}, &{{in}}, deserializeBuffer, allocator
                {%- if member.type.may_have_dawn_object -%}
                    , resolver
                {%- endif -%}
            ));
        {%- endif -%}
    {%- elif member.type.category == 'callback info' %}
        {{out}} = WGPU_{{member.type.name.SNAKE_CASE()}}_INIT;
    {%- elif not is_wire_serializable(member.type) %}
        {{out}} = nullptr;
    {%- elif member.type.name.get() == "size_t" -%}
        //* Deserializing into size_t requires check that the uint64_t used on the wire won't narrow.
        if ({{in}} > std::numeric_limits<size_t>::max()) return WireResult::FatalError;
            {{out}} = checked_cast<size_t>({{in}});
    {%- else -%}
        static_assert(sizeof({{out}}) >= sizeof({{in}}), "Deserialize assignment may not narrow.");
            {{out}} = {{in}};
    {%- endif -%}
{%- endmacro -%}

//* The main [de]serialization macro
//* Methods are very similar to structures that have one member corresponding to each arguments.
//* This macro takes advantage of the similarity to output [de]serialization code for a record
//* that is either a structure or a method, with some special cases for each.
{%- macro write_record_serialization_helpers(record, name, members, is_cmd=False, is_return_command=False) -%}
    {%- set Return = "Return" if is_return_command else "" -%}
    {%- set Cmd = "Cmd" if is_cmd else "" -%}
    {%- set Inherits = " : CmdHeader" if is_cmd else "" %}

    //* Structure for the wire format of each of the records. Members that are values
    //* are embedded directly in the structure. Other members are assumed to be in the
    //* memory directly following the structure in the buffer.
    struct {{Return}}{{name}}Transfer{{Inherits}} {
        static_assert({{[is_cmd, record.extensible, record.chained].count(True)}} <= 1,
                      "Record must be at most one of is_cmd, extensible, and chained.");
        {% if is_cmd %}
            //* Start the transfer structure with the command ID, so that casting to WireCmd gives the ID.
            {{Return}}WireCmd commandId;
        {% elif record.extensible %}
            WGPUBool hasNextInChain;
        {% elif record.chained %}
            WGPUChainedStructTransfer chain;
        {% endif %}

        {% for member in members %}
            {% if not is_wire_serializable(member.type) %}
                {% continue %}
            {% endif %}
            //* Value types are directly in the command, objects being replaced with their IDs.
            {% if member.annotation == "value" %}
                {{member_transfer_type(member)}} {{as_varName(member.name)}};
                {% continue %}
            {% endif %}
            //* Members of type "const char *" have their length embedded directly in the command.
            {% if member.length == "strlen" %}
                uint64_t {{as_varName(member.name)}}Strlen;
            {% endif %}
            //* Optional members additionally come with a boolean to indicate whether they were set.
            {% if member.optional and member.type.category != "object" %}
                WGPUBool has_{{as_varName(member.name)}};
            {% endif %}
        {% endfor %}
    };

    {% if is_cmd %}
        static_assert(offsetof({{Return}}{{name}}Transfer, commandSize) == 0);
        static_assert(offsetof({{Return}}{{name}}Transfer, commandId) == sizeof(CmdHeader));
    {% endif -%}

    {% if record.chained %}
        static_assert(offsetof({{Return}}{{name}}Transfer, chain) == 0);
    {% endif %}

    //* Returns the required transfer size for `record` in addition to the transfer structure.
    DAWN_DECLARE_UNUSED size_t {{Return}}{{name}}GetExtraRequiredSize([[maybe_unused]] const {{Return}}{{name}}{{Cmd}}& record) {
        size_t result = 0;

        //* Gather how much space will be needed for the extension chain.
        {% if record.extensible %}
            if (record.nextInChain != nullptr) {
                result += GetChainedStructExtraRequiredSize(record.nextInChain);
            }
        {% endif %}
        //* Gather space needed for pointer members.
        {% for member in members %}
            {%- set memberName = as_varName(member.name) -%}
            //* Skip size computation if we are skipping serialization.
            {% if member.skip_serialize %}
                {% continue %}
            {% endif %}
            //* Special handling of const char* that have their length embedded directly in the command.
            {% if member.length == "strlen" %}
                {% if member.optional %}
                    if (record.{{memberName}} != nullptr) {
                        result += Align(std::strlen(record.{{memberName}}), kWireBufferAlignment);
                    }
                {% else %}
                    DAWN_ASSERT(record.{{memberName}} != nullptr);
                    result += Align(std::strlen(record.{{memberName}}), kWireBufferAlignment);
                {% endif %}
                {% continue %}
            {% endif %}
            //* Normal handling for pointer members and structs.
            {% if member.annotation != "value" %}
                {% if member.type.category != "object" and member.optional %}
                    if (record.{{as_varName(member.name)}} != nullptr)
                {% endif %}
                {
                    {% do assert(member.annotation != "const*const*", "const*const* not valid here") %}
                    auto memberLength = {{member_length(member, "record.")}};
                    auto size = WireAlignSizeofN<{{member_transfer_type(member)}}>(memberLength);
                    DAWN_ASSERT(size);
                    result += *size;
                    //* Structures might contain more pointers so we need to add their extra size as well.
                    {% if member.type.category == "structure" %}
                        for (decltype(memberLength) i = 0; i < memberLength; ++i) {
                            {% do assert(member.annotation == "const*" or member.annotation == "*", "unhandled annotation: " + member.annotation)%}
                            result += {{as_cType(member.type.name)}}GetExtraRequiredSize(record.{{as_varName(member.name)}}[i]);
                        }
                    {% endif %}
                }
            {% elif member.type.category == "structure" %}
                result += {{as_cType(member.type.name)}}GetExtraRequiredSize(record.{{as_varName(member.name)}});
            {% endif %}
        {% endfor %}
        return result;
    }
    // GetExtraRequiredSize isn't used for structures that are value members of other structures
    // because we assume they cannot contain pointers themselves.
    DAWN_UNUSED_FUNC({{Return}}{{name}}GetExtraRequiredSize);

    //* Serializes `record` into `transfer`, using `buffer` to get more space for pointed-to data
    //* and `provider` to serialize objects.
    DAWN_DECLARE_UNUSED WireResult {{Return}}{{name}}Serialize(
        const {{Return}}{{name}}{{Cmd}}& record,
        {{Return}}{{name}}Transfer* transfer,
        [[maybe_unused]] SerializeBuffer* buffer
        {%- if record.may_have_dawn_object -%}
            , const ObjectIdProvider& provider
        {%- endif -%}
    ) {
        //* Handle special transfer members of methods.
        {% if is_cmd %}
            transfer->commandId = {{Return}}WireCmd::{{name}};
        {% endif %}

        {% if record.extensible %}
            if (record.nextInChain != nullptr) {
                transfer->hasNextInChain = true;
                WIRE_TRY(SerializeChainedStruct(record.nextInChain, buffer, provider));
            } else {
                transfer->hasNextInChain = false;
            }
        {% endif %}
        {% if record.chained %}
            //* Should be set by the root descriptor's call to SerializeChainedStruct.
            DAWN_ASSERT(transfer->chain.sType == {{as_cEnum(types["s type"].name, record.name)}});
            DAWN_ASSERT(transfer->chain.hasNext == (record.chain.next != nullptr));
        {% endif %}

        //* Iterate members, sorted in reverse on "attribute" so that "value" types are serialized first.
        //* Note this is important because some array pointer members rely on another "value" for their
        //* "length", but order is not always given.
        {% for member in members | sort(reverse=true, attribute="annotation") %}
            {% set memberName = as_varName(member.name) %}
            //* Skip serialization for custom serialized members and callback infos.
            {% if member.skip_serialize or member.type.category == 'callback info' %}
                {% continue %}
            {% endif %}
            //* Value types are directly in the transfer record, objects being replaced with their IDs.
            {% if member.annotation == "value" %}
                {{serialize_member(member, "record." + memberName, "transfer->" + memberName)}}
                {% continue %}
            {% endif %}
            //* Special handling of const char* that have their length embedded directly in the command.
            {% if member.length == "strlen" %}
                {% if member.optional %}
                    bool has_{{memberName}} = record.{{memberName}} != nullptr;
                    transfer->has_{{memberName}} = has_{{memberName}};
                    if (has_{{memberName}}) {
                {% else %}
                    {
                {% endif %}
                    transfer->{{memberName}}Strlen = std::strlen(record.{{memberName}});

                    char* stringInBuffer;
                    WIRE_TRY(buffer->NextN(transfer->{{memberName}}Strlen, &stringInBuffer));
                    memcpy(stringInBuffer, record.{{memberName}}, transfer->{{memberName}}Strlen);
                }
                {% continue %}
            {% endif %}
            //* Allocate space and write the non-value arguments in it.
            {% do assert(member.annotation != "const*const*") %}
            {% if member.type.category != "object" and member.optional %}
                bool has_{{memberName}} = record.{{memberName}} != nullptr;
                transfer->has_{{memberName}} = has_{{memberName}};
                if (has_{{memberName}}) {
            {% else %}
                {
            {% endif %}
                auto memberLength = {{member_length(member, "record.")}};

                {{member_transfer_type(member)}}* memberBuffer;
                WIRE_TRY(buffer->NextN(memberLength, &memberBuffer));

                {% if member.type.is_wire_transparent %}
                    //* memcpy is not defined for null pointers, even when the length is zero.
                    //* conflicts with the common practice to use (nullptr, 0) to represent an
                    //* span. Guard memcpy with a zero check to work around this language bug.
                    if (memberLength != 0) {
                        memcpy(
                            memberBuffer, record.{{memberName}},
                            {{member_transfer_sizeof(member)}} * memberLength);
                    }
                {% else %}
                    //* This loop cannot overflow because it iterates up to |memberLength|. Even if
                    //* memberLength were the maximum integer value, |i| would become equal to it
                    //* just before exiting the loop, but not increment past or wrap around.
                    for (decltype(memberLength) i = 0; i < memberLength; ++i) {
                        {{serialize_member(member, "record." + memberName + "[i]", "memberBuffer[i]" )}}
                    }
                {% endif %}
            }
        {% endfor %}

        return WireResult::Success;
    }
    DAWN_UNUSED_FUNC({{Return}}{{name}}Serialize);

    //* Deserializes `transfer` into `record` getting more serialized data from `buffer` and `size`
    //* if needed, using `allocator` to store pointed-to values and `resolver` to translate object
    //* Ids to actual objects.
    DAWN_DECLARE_UNUSED WireResult {{Return}}{{name}}Deserialize(
        {{Return}}{{name}}{{Cmd}}* record,
        const volatile {{Return}}{{name}}Transfer* transfer,
        DeserializeBuffer* deserializeBuffer,
        [[maybe_unused]] DeserializeAllocator* allocator
        {%- if record.may_have_dawn_object -%}
            , const ObjectIdResolver& resolver
        {%- endif -%}
    ) {
        {% if is_cmd %}
            DAWN_ASSERT(transfer->commandId == {{Return}}WireCmd::{{name}});
        {% endif %}
        {% if record.derived_method %}
            record->selfId = transfer->self;
        {% endif %}

        {% if record.extensible %}
            record->nextInChain = nullptr;
            if (transfer->hasNextInChain) {
                WIRE_TRY(DeserializeChainedStruct(&record->nextInChain, deserializeBuffer, allocator, resolver));
            }
        {% endif %}
        {% if record.chained %}
            //* Should be set by the root descriptor's call to DeserializeChainedStruct.
            //* Don't check |record->chain.next| matches because it is not set until the
            //* next iteration inside DeserializeChainedStruct.
            DAWN_ASSERT(record->chain.sType == {{as_cEnum(types["s type"].name, record.name)}});
            DAWN_ASSERT(record->chain.next == nullptr);
        {% endif %}

        //* Iterate members, sorted in reverse on "attribute" so that "value" types are serialized first.
        //* Note this is important because some array pointer members rely on another "value" for their
        //* "length", but order is not always given.
        {% for member in members | sort(reverse=true, attribute="annotation") %}
            {% set memberName = as_varName(member.name) %}
            //* Value types are directly in the transfer record, objects being replaced with their IDs.
            {% if member.annotation == "value" %}
                {{deserialize_member(member, "transfer->" + memberName, "record->" + memberName)}}
                {% continue %}
            {% endif %}
            //* Special handling of const char* that have their length embedded directly in the command.
            {% if member.length == "strlen" %}
                {% if member.optional %}
                    bool has_{{memberName}} = transfer->has_{{memberName}};
                    record->{{memberName}} = nullptr;
                    if (has_{{memberName}}) {
                {% else %}
                    {
                {% endif %}
                    uint64_t stringLength64 = transfer->{{memberName}}Strlen;
                    if (stringLength64 >= std::numeric_limits<size_t>::max()) {
                        //* Cannot allocate space for the string. It can be at most
                        //* size_t::max() - 1. We need 1 byte for the null-terminator.
                        return WireResult::FatalError;
                    }
                    size_t stringLength = static_cast<size_t>(stringLength64);

                    const volatile char* stringInBuffer;
                    WIRE_TRY(deserializeBuffer->ReadN(stringLength, &stringInBuffer));

                    char* copiedString;
                    WIRE_TRY(GetSpace(allocator, stringLength + 1, &copiedString));
                    //* We can cast away the volatile qualifier because DeserializeBuffer::ReadN already
                    //* validated that the range [stringInBuffer, stringInBuffer + stringLength) is valid.
                    //* memcpy may have an unknown access pattern, but this is fine since the string is only
                    //* data and won't affect control flow of this function.
                    memcpy(copiedString, const_cast<const char*>(stringInBuffer), stringLength);
                    copiedString[stringLength] = '\0';
                    record->{{memberName}} = copiedString;
                }
                {% continue %}
            {% endif %}
            //* Get extra buffer data, and copy pointed to values in extra allocated space. Note that
            //* currently, there is an implicit restriction that "skip_serialize" members must not be be
            //* a "value" type, and that they are the last non-"value" type specified in the list in
            //* dawn_wire.json.
            {% do assert(member.annotation != "const*const*") %}
            {% if member.type.category != "object" and member.optional %}
                //* Non-constant length optional members use length=0 to denote they aren't present.
                //* Otherwise we could have length=N and has_member=false, causing reads from an
                //* uninitialized pointer.
                {% do assert(member.length == "constant") %}
                bool has_{{memberName}} = transfer->has_{{memberName}};
                record->{{memberName}} = nullptr;
                if (has_{{memberName}}) {
            {% else %}
                {
            {% endif %}
                auto memberLength = {{member_length(member, "record->")}};
                const volatile {{member_transfer_type(member)}}* memberBuffer;
                WIRE_TRY(deserializeBuffer->ReadN(memberLength, &memberBuffer));

                //* For data-only members (e.g. "data" in WriteBuffer and WriteTexture), they are
                //* not security sensitive so we can directly refer the data inside the transfer
                //* buffer in dawn_native. For other members, as prevention of TOCTOU attacks is an
                //* important feature of the wire, we must make sure every single value returned to
                //* dawn_native must be a copy of what's in the wire.
                {% if member.json_data["wire_is_data_only"] %}
                    record->{{memberName}} =
                        const_cast<const {{member_transfer_type(member)}}*>(memberBuffer);

                {% else %}
                    {{as_cType(member.type.name)}}* copiedMembers;
                    WIRE_TRY(GetSpace(allocator, memberLength, &copiedMembers));
                    record->{{memberName}} = copiedMembers;

                    {% if member.type.is_wire_transparent %}
                        //* memcpy is not defined for null pointers, even when the length is zero.
                        //* conflicts with the common practice to use (nullptr, 0) to represent an
                        //* span. Guard memcpy with a zero check to work around this language bug.
                        if (memberLength != 0) {
                            //* memcpy is not allowed to copy from volatile objects. However, these
                            //* arrays are just used as plain data, and don't impact control flow.
                            //* So if the underlying data were changed while the copy was still
                            //* executing, we would get different data - but it wouldn't cause
                            //* unexpected downstream effects.
                            memcpy(
                                copiedMembers,
                                const_cast<const {{member_transfer_type(member)}}*>(memberBuffer),
                              {{member_transfer_sizeof(member)}} * memberLength);
                        }
                    {% else %}
                        //* This loop cannot overflow because it iterates up to |memberLength|. Even
                        //* if memberLength were the maximum integer value, |i| would become equal
                        //* to it just before exiting the loop, but not increment past or wrap
                        //* around.
                        for (decltype(memberLength) i = 0; i < memberLength; ++i) {
                            {{deserialize_member(member, "memberBuffer[i]", "copiedMembers[i]")}}
                        }
                    {% endif %}
                {% endif %}
            }
        {% endfor %}

        return WireResult::Success;
    }
    DAWN_UNUSED_FUNC({{Return}}{{name}}Deserialize);
{%- endmacro -%}

{%- macro write_command_serialization_methods(command, is_return) -%}
    {% set Return = "Return" if is_return else "" %}
    {% set Name = Return + command.name.CamelCase() %}
    {% set Cmd = Name + "Cmd" %}
    size_t {{Cmd}}::GetRequiredSize() const {
        return WireAlignSizeof<{{Name}}Transfer>() + {{Name}}GetExtraRequiredSize(*this);
    }

    {% if command.may_have_dawn_object %}
        WireResult {{Cmd}}::Serialize(
            size_t commandSize,
            SerializeBuffer* serializeBuffer,
            const ObjectIdProvider& provider) const {
            {{Name}}Transfer* transfer;
            WIRE_TRY(serializeBuffer->Next(&transfer));
            transfer->commandSize = commandSize;
            return ({{Name}}Serialize(*this, transfer, serializeBuffer, provider));
        }
        WireResult {{Cmd}}::Serialize(size_t commandSize, SerializeBuffer* serializeBuffer) const {
            ErrorObjectIdProvider provider;
            return Serialize(commandSize, serializeBuffer, provider);
        }

        WireResult {{Cmd}}::Deserialize(
            DeserializeBuffer* deserializeBuffer,
            DeserializeAllocator* allocator,
            const ObjectIdResolver& resolver) {
            const volatile {{Name}}Transfer* transfer;
            WIRE_TRY(deserializeBuffer->Read(&transfer));
            return {{Name}}Deserialize(this, transfer, deserializeBuffer, allocator, resolver);
        }
        WireResult {{Cmd}}::Deserialize(DeserializeBuffer* deserializeBuffer, DeserializeAllocator* allocator) {
            ErrorObjectIdResolver resolver;
            return Deserialize(deserializeBuffer, allocator, resolver);
        }
    {% else %}
        WireResult {{Cmd}}::Serialize(size_t commandSize, SerializeBuffer* serializeBuffer) const {
            {{Name}}Transfer* transfer;
            WIRE_TRY(serializeBuffer->Next(&transfer));
            transfer->commandSize = commandSize;
            return ({{Name}}Serialize(*this, transfer, serializeBuffer));
        }
        WireResult {{Cmd}}::Serialize(
            size_t commandSize,
            SerializeBuffer* serializeBuffer,
            const ObjectIdProvider&) const {
            return Serialize(commandSize, serializeBuffer);
        }

        WireResult {{Cmd}}::Deserialize(DeserializeBuffer* deserializeBuffer, DeserializeAllocator* allocator) {
            const volatile {{Name}}Transfer* transfer;
            WIRE_TRY(deserializeBuffer->Read(&transfer));
            return {{Name}}Deserialize(this, transfer, deserializeBuffer, allocator);
        }
        WireResult {{Cmd}}::Deserialize(
            DeserializeBuffer* deserializeBuffer,
            DeserializeAllocator* allocator,
            const ObjectIdResolver&) {
            return Deserialize(deserializeBuffer, allocator);
        }
    {% endif %}
{%- endmacro -%}

{%- macro make_chained_struct_serialization_helpers(out=None) %}
    {% set ChainedStructPtr = "WGPUChainedStructOut*" if out else "const WGPUChainedStruct*" %}
    {% set ChainedStruct = "WGPUChainedStructOut" if out else "WGPUChainedStruct" %}
    //* Generate the list of sTypes that we need to handle.
    {% set sTypes = [] %}
    {% for sType in types["s type"].values %}
        {% if not sType.valid %}
            {% continue %}
        {% elif sType.name.CamelCase() in client_side_structures %}
            {% continue %}
        {% elif types[sType.name.get()].output != out %}
            {% continue %}
        {% endif %}
        {% do sTypes.append(sType) %}
    {% endfor %}

    size_t GetChainedStructExtraRequiredSize({{ChainedStructPtr}} chainedStruct) {
        DAWN_ASSERT(chainedStruct != nullptr);
        size_t result = 0;
        while (chainedStruct != nullptr) {
            switch (chainedStruct->sType) {
                {% for sType in sTypes %}
                    case {{as_cEnum(types["s type"].name, sType.name)}}: {
                        const auto& typedStruct = *reinterpret_cast<{{as_cType(sType.name)}} const *>(chainedStruct);
                        result += WireAlignSizeof<{{as_cType(sType.name)}}Transfer>();
                        result += {{as_cType(sType.name)}}GetExtraRequiredSize(typedStruct);
                        chainedStruct = typedStruct.chain.next;
                        break;
                    }
                {% endfor %}
                default:
                    // Invalid enum. Reserve space just for the transfer header (sType and hasNext).
                    result += WireAlignSizeof<WGPUChainedStructTransfer>();
                    chainedStruct = chainedStruct->next;
                    break;
            }
        }
        return result;
    }

    [[nodiscard]] WireResult SerializeChainedStruct({{ChainedStructPtr}} chainedStruct,
                                                    SerializeBuffer* buffer,
                                                    const ObjectIdProvider& provider) {
        DAWN_ASSERT(chainedStruct != nullptr);
        DAWN_ASSERT(buffer != nullptr);
        do {
            switch (chainedStruct->sType) {
                {% for sType in sTypes %}
                    {% set CType = as_cType(sType.name) %}
                    case {{as_cEnum(types["s type"].name, sType.name)}}: {
                        {{CType}}Transfer* transfer;
                        WIRE_TRY(buffer->Next(&transfer));
                        transfer->chain.sType = chainedStruct->sType;
                        transfer->chain.hasNext = chainedStruct->next != nullptr;

                        WIRE_TRY({{CType}}Serialize(*reinterpret_cast<{{CType}} const*>(chainedStruct), transfer, buffer
                            {%- if types[sType.name.get()].may_have_dawn_object -%}
                            , provider
                            {%- endif -%}
                        ));

                        chainedStruct = chainedStruct->next;
                    } break;
                {% endfor %}
                default: {
                    // Invalid enum. Serialize just the transfer header with Invalid as the sType.
                    // TODO(crbug.com/dawn/369): Unknown sTypes are silently discarded.
                    if (chainedStruct->sType != WGPUSType(0)) {
                        dawn::WarningLog() << "Unknown sType " << chainedStruct->sType << " discarded.";
                    }

                    WGPUChainedStructTransfer* transfer;
                    WIRE_TRY(buffer->Next(&transfer));
                    transfer->sType = WGPUSType(0);
                    transfer->hasNext = chainedStruct->next != nullptr;

                    // Still move on in case there are valid structs after this.
                    chainedStruct = chainedStruct->next;
                    break;
                }
            }
        } while (chainedStruct != nullptr);
        return WireResult::Success;
    }

    WireResult DeserializeChainedStruct({{ChainedStructPtr}}* outChainNext,
                                        DeserializeBuffer* deserializeBuffer,
                                        DeserializeAllocator* allocator,
                                        const ObjectIdResolver& resolver) {
        bool hasNext;
        do {
            const volatile WGPUChainedStructTransfer* header;
            WIRE_TRY(deserializeBuffer->Peek(&header));
            WGPUSType sType = header->sType;
            switch (sType) {
                {% for sType in sTypes %}
                    {% set CType = as_cType(sType.name) %}
                    case {{as_cEnum(types["s type"].name, sType.name)}}: {
                        const volatile {{CType}}Transfer* transfer;
                        WIRE_TRY(deserializeBuffer->Read(&transfer));

                        {{CType}}* outStruct;
                        WIRE_TRY(GetSpace(allocator, 1u, &outStruct));
                        outStruct->chain.sType = sType;
                        outStruct->chain.next = nullptr;

                        *outChainNext = &outStruct->chain;
                        outChainNext = &outStruct->chain.next;

                        WIRE_TRY({{CType}}Deserialize(outStruct, transfer, deserializeBuffer, allocator
                            {%- if types[sType.name.get()].may_have_dawn_object -%}
                                , resolver
                            {%- endif -%}
                        ));

                        hasNext = transfer->chain.hasNext;
                    } break;
                {% endfor %}
                default: {
                    // Invalid enum. Deserialize just the transfer header with Invalid as the sType.
                    // TODO(crbug.com/dawn/369): Unknown sTypes are silently discarded.
                    if (sType != WGPUSType(0)) {
                        dawn::WarningLog() << "Unknown sType " << sType << " discarded.";
                    }

                    const volatile WGPUChainedStructTransfer* transfer;
                    WIRE_TRY(deserializeBuffer->Read(&transfer));

                    {{ChainedStruct}}* outStruct;
                    WIRE_TRY(GetSpace(allocator, 1u, &outStruct));
                    outStruct->sType = WGPUSType(0);
                    outStruct->next = nullptr;

                    // Still move on in case there are valid structs after this.
                    *outChainNext = outStruct;
                    outChainNext = &outStruct->next;
                    hasNext = transfer->hasNext;
                    break;
                }
            }
        } while (hasNext);
        return WireResult::Success;
    }
{% endmacro %}


namespace dawn::wire {
namespace {

// Allocates enough space from allocator to countain T[count] and return it in out.
// Return FatalError if the allocator couldn't allocate the memory.
// Always writes to |out| on success.
template <typename T, typename N>
WireResult GetSpace(DeserializeAllocator* allocator, N count, T** out) {
    // Because we use this function extensively when `count` == 1, we can optimize the
    // size computations a bit more for those cases via constexpr version of the
    // alignment computation.
    constexpr size_t kSizeofT = WireAlignSizeof<T>();
    size_t size = 0;
    if (count == 1) {
      size = kSizeofT;
    } else {
      auto sizeN = WireAlignSizeofN<T>(count);
      // A size of 0 indicates an overflow, so return an error.
      if (!sizeN) {
        return WireResult::FatalError;
      }
      size = *sizeN;
    }

    *out = static_cast<T*>(allocator->GetSpace(size));
    if (*out == nullptr) {
        return WireResult::FatalError;
    }

    return WireResult::Success;
}

struct WGPUChainedStructTransfer {
    WGPUSType sType;
    bool hasNext;
};

size_t GetChainedStructExtraRequiredSize(const WGPUChainedStruct* chainedStruct);
[[nodiscard]] WireResult SerializeChainedStruct(const WGPUChainedStruct* chainedStruct,
                                                  SerializeBuffer* buffer,
                                                  const ObjectIdProvider& provider);
WireResult DeserializeChainedStruct(const WGPUChainedStruct** outChainNext,
                                    DeserializeBuffer* deserializeBuffer,
                                    DeserializeAllocator* allocator,
                                    const ObjectIdResolver& resolver);

size_t GetChainedStructExtraRequiredSize(WGPUChainedStructOut* chainedStruct);
[[nodiscard]] WireResult SerializeChainedStruct(WGPUChainedStructOut* chainedStruct,
                                                  SerializeBuffer* buffer,
                                                  const ObjectIdProvider& provider);
WireResult DeserializeChainedStruct(WGPUChainedStructOut** outChainNext,
                                    DeserializeBuffer* deserializeBuffer,
                                    DeserializeAllocator* allocator,
                                    const ObjectIdResolver& resolver);

// Manually define serialization and deserialization for WGPUStringView because
// it has a special encoding where:
//  { .data = nullptr, .length = SIZE_MAX }  --> nil
//  { .data = non-null, .length = SIZE_MAX } --> null-terminated, use strlen
//  { .data = ..., .length = 0 }             --> ""
//  { .data = ..., .length > 0 }             --> string of size `length`
struct WGPUStringViewTransfer {
    bool has_data;
    uint64_t length;
};

size_t WGPUStringViewGetExtraRequiredSize(const WGPUStringView& record) {
    size_t size = record.length;
    if (size == SIZE_MAX) {
        // This is a null-terminated string, or it's nil.
        size = record.data ? std::strlen(record.data) : 0;
    }
    return Align(size, kWireBufferAlignment);
}

WireResult WGPUStringViewSerialize(
    const WGPUStringView& record,
    WGPUStringViewTransfer* transfer,
    SerializeBuffer* buffer) {

    bool has_data = record.data != nullptr;
    uint64_t length = record.length;
    transfer->has_data = has_data;

    if (!has_data) {
        transfer->length = length;
        return WireResult::Success;
    }
    if (length == SIZE_MAX) {
        length = std::strlen(record.data);
    }
    if (length > 0) {
        char* memberBuffer;
        WIRE_TRY(buffer->NextN(length, &memberBuffer));
        memcpy(memberBuffer, record.data, length);
    }
    transfer->length = length;
    return WireResult::Success;
}

WireResult WGPUStringViewDeserialize(
    WGPUStringView* record,
    const volatile WGPUStringViewTransfer* transfer,
    DeserializeBuffer* deserializeBuffer,
    DeserializeAllocator* allocator) {

    bool has_data = transfer->has_data;
    uint64_t length = transfer->length;

    if (length > SIZE_MAX) {
        return WireResult::FatalError;
    }
    if (!has_data) {
        record->data = nullptr;
        if (length != 0 && length != SIZE_MAX) {
            // Invalid string.
            return WireResult::FatalError;
        }
        record->length = static_cast<size_t>(length);
        return WireResult::Success;
    }
    if (length == 0) {
        record->data = "";
        record->length = 0;
        return WireResult::Success;
    }

    size_t stringLength = static_cast<size_t>(length);
    const volatile char* stringInBuffer;
    WIRE_TRY(deserializeBuffer->ReadN(stringLength, &stringInBuffer));

    char* copiedString;
    WIRE_TRY(GetSpace(allocator, stringLength, &copiedString));
    memcpy(copiedString, const_cast<const char*>(stringInBuffer), stringLength);

    record->data = copiedString;
    record->length = stringLength;
    return WireResult::Success;
}

//* Output structure [de]serialization first because it is used by commands.
{% for type in by_category["structure"] %}
    {%- set name = as_cType(type.name) -%}
    {% if type.name.CamelCase() not in client_side_structures and name != "WGPUStringView" -%}
        {{write_record_serialization_helpers(type, name, type.members, is_cmd=False)}}
    {% endif %}
{% endfor %}

{{make_chained_struct_serialization_helpers(out=False)}}
{{make_chained_struct_serialization_helpers(out=True)}}

//* Output [de]serialization helpers for commands
{% for command in cmd_records["command"] %}
    {%- set name = command.name.CamelCase() -%}
    {{write_record_serialization_helpers(command, name, command.members, is_cmd=True)}}
{% endfor %}

//* Output [de]serialization helpers for return commands
{% for command in cmd_records["return command"] %}
    {%- set name = command.name.CamelCase() -%}
    {{write_record_serialization_helpers(command, name, command.members,
                                         is_cmd=True, is_return_command=True)}}
{% endfor %}

// Implementation of ObjectIdResolver that always errors.
// Used when the generator adds a provider argument because of a chained
// struct, but in practice, a chained struct in that location is invalid.
class ErrorObjectIdResolver final : public ObjectIdResolver {
    public:
    {% for type in by_category["object"] %}
      WireResult GetFromId(ObjectId id, {{as_cType(type.name)}}* out) const override {
          return WireResult::FatalError;
      }
      WireResult GetOptionalFromId(ObjectId id, {{as_cType(type.name)}}* out) const override {
          return WireResult::FatalError;
      }
    {% endfor %}
};

// Implementation of ObjectIdProvider that always errors.
// Used when the generator adds a provider argument because of a chained
// struct, but in practice, a chained struct in that location is invalid.
class ErrorObjectIdProvider final : public ObjectIdProvider {
    public:
    {% for type in by_category["object"] %}
      WireResult GetId({{as_cType(type.name)}} object, ObjectId* out) const override {
          return WireResult::FatalError;
      }
      WireResult GetOptionalId({{as_cType(type.name)}} object, ObjectId* out) const override {
          return WireResult::FatalError;
      }
    {% endfor %}
};

}  // anonymous namespace

{% for command in cmd_records["command"] -%}
    {{write_command_serialization_methods(command, False)}}
{% endfor %}

{% for command in cmd_records["return command"] -%}
    {{write_command_serialization_methods(command, True)}}
{% endfor %}

}  // namespace dawn::wire
