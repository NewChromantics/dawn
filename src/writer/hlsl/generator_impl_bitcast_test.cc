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

#include <memory>

#include "src/ast/bitcast_expression.h"
#include "src/ast/identifier_expression.h"
#include "src/ast/module.h"
#include "src/ast/type/f32_type.h"
#include "src/ast/type/i32_type.h"
#include "src/ast/type/u32_type.h"
#include "src/writer/hlsl/test_helper.h"

namespace tint {
namespace writer {
namespace hlsl {
namespace {

using HlslGeneratorImplTest_Bitcast = TestHelper;

TEST_F(HlslGeneratorImplTest_Bitcast, EmitExpression_Bitcast_Float) {
  auto* id = Expr("id");
  auto* bitcast = create<ast::BitcastExpression>(ty.f32, id);

  ASSERT_TRUE(gen.EmitExpression(pre, out, bitcast)) << gen.error();
  EXPECT_EQ(result(), "asfloat(test_id)");
}

TEST_F(HlslGeneratorImplTest_Bitcast, EmitExpression_Bitcast_Int) {
  auto* id = Expr("id");
  auto* bitcast = create<ast::BitcastExpression>(ty.i32, id);

  ASSERT_TRUE(gen.EmitExpression(pre, out, bitcast)) << gen.error();
  EXPECT_EQ(result(), "asint(test_id)");
}

TEST_F(HlslGeneratorImplTest_Bitcast, EmitExpression_Bitcast_Uint) {
  auto* id = Expr("id");
  auto* bitcast = create<ast::BitcastExpression>(ty.u32, id);

  ASSERT_TRUE(gen.EmitExpression(pre, out, bitcast)) << gen.error();
  EXPECT_EQ(result(), "asuint(test_id)");
}

}  // namespace
}  // namespace hlsl
}  // namespace writer
}  // namespace tint
