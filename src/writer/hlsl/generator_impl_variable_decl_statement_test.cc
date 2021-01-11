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
#include <vector>

#include "src/ast/identifier_expression.h"
#include "src/ast/module.h"
#include "src/ast/type/array_type.h"
#include "src/ast/type/f32_type.h"
#include "src/ast/type/matrix_type.h"
#include "src/ast/type/vector_type.h"
#include "src/ast/variable.h"
#include "src/ast/variable_decl_statement.h"
#include "src/writer/hlsl/test_helper.h"

namespace tint {
namespace writer {
namespace hlsl {
namespace {

using HlslGeneratorImplTest_VariableDecl = TestHelper;

TEST_F(HlslGeneratorImplTest_VariableDecl, Emit_VariableDeclStatement) {
  auto* var = Var("a", ast::StorageClass::kNone, ty.f32);

  auto* stmt = create<ast::VariableDeclStatement>(var);
  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), "  float test_a;\n");
}

TEST_F(HlslGeneratorImplTest_VariableDecl, Emit_VariableDeclStatement_Const) {
  auto* var = Const("a", ast::StorageClass::kNone, ty.f32);

  auto* stmt = create<ast::VariableDeclStatement>(var);
  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), "  const float test_a;\n");
}

TEST_F(HlslGeneratorImplTest_VariableDecl, Emit_VariableDeclStatement_Array) {
  auto* var = Var("a", ast::StorageClass::kNone, ty.array<f32, 5>());

  auto* stmt = create<ast::VariableDeclStatement>(var);
  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), "  float test_a[5];\n");
}

TEST_F(HlslGeneratorImplTest_VariableDecl,
       Emit_VariableDeclStatement_Function) {
  auto* var = Var("a", ast::StorageClass::kFunction, ty.f32);

  auto* stmt = create<ast::VariableDeclStatement>(var);
  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), "  float test_a;\n");
}

TEST_F(HlslGeneratorImplTest_VariableDecl, Emit_VariableDeclStatement_Private) {
  auto* var = Var("a", ast::StorageClass::kPrivate, ty.f32);

  auto* stmt = create<ast::VariableDeclStatement>(var);
  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), "  float test_a;\n");
}

TEST_F(HlslGeneratorImplTest_VariableDecl,
       Emit_VariableDeclStatement_Initializer_Private) {
  auto* var = Var("a", ast::StorageClass::kNone, ty.f32, Expr("initializer"),
                  ast::VariableDecorationList{});

  auto* stmt = create<ast::VariableDeclStatement>(var);
  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), R"(float test_a = test_initializer;
)");
}

TEST_F(HlslGeneratorImplTest_VariableDecl,
       Emit_VariableDeclStatement_Initializer_ZeroVec) {
  auto* var = Var("a", ast::StorageClass::kNone, ty.vec3<f32>(), vec3<f32>(),
                  ast::VariableDecorationList{});

  auto* stmt = create<ast::VariableDeclStatement>(var);
  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(), R"(float3 test_a = float3(0.0f);
)");
}

TEST_F(HlslGeneratorImplTest_VariableDecl,
       Emit_VariableDeclStatement_Initializer_ZeroMat) {
  auto* var = Var("a", ast::StorageClass::kNone, ty.mat2x3<f32>(),
                  mat2x3<f32>(), ast::VariableDecorationList{});

  auto* stmt = create<ast::VariableDeclStatement>(var);
  ASSERT_TRUE(gen.EmitStatement(out, stmt)) << gen.error();
  EXPECT_EQ(result(),
            R"(float3x2 test_a = float3x2(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
)");
}

}  // namespace
}  // namespace hlsl
}  // namespace writer
}  // namespace tint
