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

#include "src/writer/wgsl/generator_impl.h"

#include <memory>

#include "gtest/gtest.h"
#include "src/ast/function.h"
#include "src/ast/module.h"
#include "src/ast/type/void_type.h"
#include "src/writer/wgsl/test_helper.h"

namespace tint {
namespace writer {
namespace wgsl {
namespace {

using WgslGeneratorImplTest = TestHelper;

TEST_F(WgslGeneratorImplTest, Generate) {
  ast::type::Void void_type;

  mod.AddFunction(create<ast::Function>(
      Source{}, mod.RegisterSymbol("a_func"), "my_func", ast::VariableList{},
      &void_type, create<ast::BlockStatement>(Source{}),
      ast::FunctionDecorationList{}));

  ASSERT_TRUE(gen.Generate(mod)) << gen.error();
  EXPECT_EQ(gen.result(), R"(fn my_func() -> void {
}

)");
}

}  // namespace
}  // namespace wgsl
}  // namespace writer
}  // namespace tint
