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

#include "src/tint/lang/wgsl/ast/helper_test.h"

namespace tint::ast {
namespace {

using namespace tint::core::number_suffixes;  // NOLINT

using TemplatedIdentifierTest = TestHelper;

TEST_F(TemplatedIdentifierTest, Creation) {
    auto* i = Ident("ident", 1_a, Add("x", "y"), false, "x");
    EXPECT_EQ(i->symbol, Symbols().Get("ident"));
    auto* t = i->As<TemplatedIdentifier>();
    ASSERT_NE(t, nullptr);
    ASSERT_EQ(t->arguments.Length(), 4u);
    EXPECT_TRUE(t->arguments[0]->Is<IntLiteralExpression>());
    EXPECT_TRUE(t->arguments[1]->Is<BinaryExpression>());
    EXPECT_TRUE(t->arguments[2]->Is<BoolLiteralExpression>());
    EXPECT_TRUE(t->arguments[3]->Is<IdentifierExpression>());
}

TEST_F(TemplatedIdentifierTest, Creation_WithSource) {
    auto* i = Ident(Source{{20, 2}}, "ident", 1_a, Add("x", "y"), false, "x");
    EXPECT_EQ(i->symbol, Symbols().Get("ident"));
    auto* t = i->As<TemplatedIdentifier>();
    ASSERT_NE(t, nullptr);
    ASSERT_EQ(t->arguments.Length(), 4u);
    EXPECT_TRUE(t->arguments[0]->Is<IntLiteralExpression>());
    EXPECT_TRUE(t->arguments[1]->Is<BinaryExpression>());
    EXPECT_TRUE(t->arguments[2]->Is<BoolLiteralExpression>());
    EXPECT_TRUE(t->arguments[3]->Is<IdentifierExpression>());

    auto src = i->source;
    EXPECT_EQ(src.range.begin.line, 20u);
    EXPECT_EQ(src.range.begin.column, 2u);
}

TEST_F(TemplatedIdentifierTest, Assert_InvalidSymbol) {
    EXPECT_DEATH(
        {
            ProgramBuilder b;
            b.Expr("");
        },
        "internal compiler error");
}

TEST_F(TemplatedIdentifierTest, Assert_DifferentGenerationID_Symbol) {
    EXPECT_DEATH(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.Ident(b2.Sym("b2"), b1.Expr(1_i));
        },
        "internal compiler error");
}

TEST_F(TemplatedIdentifierTest, Assert_DifferentGenerationID_TemplateArg) {
    EXPECT_DEATH(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.Ident("b1", b2.Expr(1_i));
        },
        "internal compiler error");
}

}  // namespace
}  // namespace tint::ast
