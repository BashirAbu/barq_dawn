// Copyright 2020 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/ast/struct_member.h"

#include <sstream>
#include <utility>

#include "src/ast/struct_member_offset_decoration.h"
#include "src/ast/test_helper.h"
#include "src/ast/type/i32_type.h"

namespace tint {
namespace ast {
namespace {

using StructMemberTest = TestHelper;

TEST_F(StructMemberTest, Creation) {
  StructMemberDecorationList decorations;
  decorations.emplace_back(create<StructMemberOffsetDecoration>(4));

  auto* st =
      create<StructMember>(mod->RegisterSymbol("a"), "a", ty.i32, decorations);
  EXPECT_EQ(st->symbol(), Symbol(1));
  EXPECT_EQ(st->name(), "a");
  EXPECT_EQ(st->type(), ty.i32);
  EXPECT_EQ(st->decorations().size(), 1u);
  EXPECT_TRUE(st->decorations()[0]->Is<StructMemberOffsetDecoration>());
  EXPECT_EQ(st->source().range.begin.line, 0u);
  EXPECT_EQ(st->source().range.begin.column, 0u);
  EXPECT_EQ(st->source().range.end.line, 0u);
  EXPECT_EQ(st->source().range.end.column, 0u);
}

TEST_F(StructMemberTest, CreationWithSource) {
  auto* st = create<StructMember>(
      Source{Source::Range{Source::Location{27, 4}, Source::Location{27, 8}}},
      mod->RegisterSymbol("a"), "a", ty.i32, StructMemberDecorationList{});
  EXPECT_EQ(st->symbol(), Symbol(1));
  EXPECT_EQ(st->name(), "a");
  EXPECT_EQ(st->type(), ty.i32);
  EXPECT_EQ(st->decorations().size(), 0u);
  EXPECT_EQ(st->source().range.begin.line, 27u);
  EXPECT_EQ(st->source().range.begin.column, 4u);
  EXPECT_EQ(st->source().range.end.line, 27u);
  EXPECT_EQ(st->source().range.end.column, 8u);
}

TEST_F(StructMemberTest, IsValid) {
  auto* st = create<StructMember>(mod->RegisterSymbol("a"), "a", ty.i32,
                                  StructMemberDecorationList{});
  EXPECT_TRUE(st->IsValid());
}

TEST_F(StructMemberTest, IsValid_EmptySymbol) {
  auto* st = create<StructMember>(mod->RegisterSymbol(""), "", ty.i32,
                                  StructMemberDecorationList{});
  EXPECT_FALSE(st->IsValid());
}

TEST_F(StructMemberTest, IsValid_NullType) {
  auto* st = create<StructMember>(mod->RegisterSymbol("a"), "a", nullptr,
                                  StructMemberDecorationList{});
  EXPECT_FALSE(st->IsValid());
}

TEST_F(StructMemberTest, IsValid_Null_Decoration) {
  StructMemberDecorationList decorations;
  decorations.emplace_back(create<StructMemberOffsetDecoration>(4));
  decorations.push_back(nullptr);

  auto* st =
      create<StructMember>(mod->RegisterSymbol("a"), "a", ty.i32, decorations);
  EXPECT_FALSE(st->IsValid());
}

TEST_F(StructMemberTest, ToStr) {
  StructMemberDecorationList decorations;
  decorations.emplace_back(create<StructMemberOffsetDecoration>(4));

  auto* st =
      create<StructMember>(mod->RegisterSymbol("a"), "a", ty.i32, decorations);
  std::ostringstream out;
  st->to_str(out, 2);
  EXPECT_EQ(demangle(out.str()), "  StructMember{[[ offset 4 ]] a: __i32}\n");
}

TEST_F(StructMemberTest, ToStrNoDecorations) {
  auto* st = create<StructMember>(mod->RegisterSymbol("a"), "a", ty.i32,
                                  StructMemberDecorationList{});
  std::ostringstream out;
  st->to_str(out, 2);
  EXPECT_EQ(demangle(out.str()), "  StructMember{a: __i32}\n");
}

}  // namespace
}  // namespace ast
}  // namespace tint
