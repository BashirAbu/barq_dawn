// Copyright 2023 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/core/attribute.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "src/tint/lang/core/attribute.h"

namespace tint::core {

/// ParseAttribute parses a Attribute from a string.
/// @param str the string to parse
/// @returns the parsed enum, or Attribute::kUndefined if the string could not be parsed.
Attribute ParseAttribute(std::string_view str) {
    if (str == "align") {
        return Attribute::kAlign;
    }
    if (str == "binding") {
        return Attribute::kBinding;
    }
    if (str == "blend_src") {
        return Attribute::kBlendSrc;
    }
    if (str == "builtin") {
        return Attribute::kBuiltin;
    }
    if (str == "color") {
        return Attribute::kColor;
    }
    if (str == "compute") {
        return Attribute::kCompute;
    }
    if (str == "diagnostic") {
        return Attribute::kDiagnostic;
    }
    if (str == "fragment") {
        return Attribute::kFragment;
    }
    if (str == "group") {
        return Attribute::kGroup;
    }
    if (str == "id") {
        return Attribute::kId;
    }
    if (str == "index") {
        return Attribute::kIndex;
    }
    if (str == "interpolate") {
        return Attribute::kInterpolate;
    }
    if (str == "invariant") {
        return Attribute::kInvariant;
    }
    if (str == "location") {
        return Attribute::kLocation;
    }
    if (str == "must_use") {
        return Attribute::kMustUse;
    }
    if (str == "size") {
        return Attribute::kSize;
    }
    if (str == "vertex") {
        return Attribute::kVertex;
    }
    if (str == "workgroup_size") {
        return Attribute::kWorkgroupSize;
    }
    return Attribute::kUndefined;
}

std::string_view ToString(Attribute value) {
    switch (value) {
        case Attribute::kUndefined:
            return "undefined";
        case Attribute::kAlign:
            return "align";
        case Attribute::kBinding:
            return "binding";
        case Attribute::kBlendSrc:
            return "blend_src";
        case Attribute::kBuiltin:
            return "builtin";
        case Attribute::kColor:
            return "color";
        case Attribute::kCompute:
            return "compute";
        case Attribute::kDiagnostic:
            return "diagnostic";
        case Attribute::kFragment:
            return "fragment";
        case Attribute::kGroup:
            return "group";
        case Attribute::kId:
            return "id";
        case Attribute::kIndex:
            return "index";
        case Attribute::kInterpolate:
            return "interpolate";
        case Attribute::kInvariant:
            return "invariant";
        case Attribute::kLocation:
            return "location";
        case Attribute::kMustUse:
            return "must_use";
        case Attribute::kSize:
            return "size";
        case Attribute::kVertex:
            return "vertex";
        case Attribute::kWorkgroupSize:
            return "workgroup_size";
    }
    return "<unknown>";
}

}  // namespace tint::core
