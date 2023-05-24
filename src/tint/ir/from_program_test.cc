// Copyright 2023 The Tint Authors.
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

#include "src/tint/ir/test_helper.h"

#include "gmock/gmock.h"
#include "src/tint/ast/case_selector.h"
#include "src/tint/ast/int_literal_expression.h"
#include "src/tint/constant/scalar.h"
#include "src/tint/ir/block.h"
#include "src/tint/ir/function_terminator.h"
#include "src/tint/ir/if.h"
#include "src/tint/ir/loop.h"
#include "src/tint/ir/switch.h"

namespace tint::ir {
namespace {

/// Looks for the flow node with the given type T.
/// If no flow node is found, then nullptr is returned.
/// If multiple flow nodes are found with the type T, then an error is raised and the first is
/// returned.
template <typename T>
T* FindSingleValue(Module& mod) {
    T* found = nullptr;
    size_t count = 0;
    for (auto* node : mod.values.Objects()) {
        if (auto* as = node->As<T>()) {
            count++;
            if (!found) {
                found = as;
            }
        }
    }
    if (count > 1) {
        ADD_FAILURE() << "FindSingleValue() found " << count << " nodes of type "
                      << utils::TypeInfo::Of<T>().name;
    }
    return found;
}

using namespace tint::number_suffixes;  // NOLINT

using IR_BuilderImplTest = TestHelper;

TEST_F(IR_BuilderImplTest, Func) {
    Func("f", utils::Empty, ty.void_(), utils::Empty);

    auto m = Build();
    ASSERT_TRUE(m) << (!m ? m.Failure() : "");

    ASSERT_EQ(1u, m->functions.Length());

    auto* f = m->functions[0];
    ASSERT_NE(f->StartTarget(), nullptr);
    ASSERT_NE(f->EndTarget(), nullptr);

    EXPECT_EQ(1u, f->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(m->functions[0]->Stage(), Function::PipelineStage::kUndefined);

    EXPECT_EQ(Disassemble(m.Get()), R"(%fn1 = func f():void -> %fn2 {
  %fn2 = block {
    br %fn3  # return
  }
  %fn3 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Func_WithParam) {
    Func("f", utils::Vector{Param("a", ty.u32())}, ty.u32(), utils::Vector{Return("a")});

    auto m = Build();
    ASSERT_TRUE(m) << (!m ? m.Failure() : "");

    ASSERT_EQ(1u, m->functions.Length());

    auto* f = m->functions[0];
    ASSERT_NE(f->StartTarget(), nullptr);
    ASSERT_NE(f->EndTarget(), nullptr);

    EXPECT_EQ(1u, f->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(m->functions[0]->Stage(), Function::PipelineStage::kUndefined);

    EXPECT_EQ(Disassemble(m.Get()), R"(%fn1 = func f(%a:u32):u32 -> %fn2 {
  %fn2 = block {
    br %fn3 %a  # return
  }
  %fn3 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Func_WithMultipleParam) {
    Func("f", utils::Vector{Param("a", ty.u32()), Param("b", ty.i32()), Param("c", ty.bool_())},
         ty.void_(), utils::Empty);

    auto m = Build();
    ASSERT_TRUE(m) << (!m ? m.Failure() : "");

    ASSERT_EQ(1u, m->functions.Length());

    auto* f = m->functions[0];
    ASSERT_NE(f->StartTarget(), nullptr);
    ASSERT_NE(f->EndTarget(), nullptr);

    EXPECT_EQ(1u, f->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(m->functions[0]->Stage(), Function::PipelineStage::kUndefined);

    EXPECT_EQ(Disassemble(m.Get()), R"(%fn1 = func f(%a:u32, %b:i32, %c:bool):void -> %fn2 {
  %fn2 = block {
    br %fn3  # return
  }
  %fn3 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, EntryPoint) {
    Func("f", utils::Empty, ty.void_(), utils::Empty,
         utils::Vector{Stage(ast::PipelineStage::kFragment)});

    auto m = Build();
    ASSERT_TRUE(m) << (!m ? m.Failure() : "");

    EXPECT_EQ(m->functions[0]->Stage(), Function::PipelineStage::kFragment);
}

TEST_F(IR_BuilderImplTest, IfStatement) {
    auto* ast_if = If(true, Block(), Else(Block()));
    WrapInFunction(ast_if);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(1u, flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->False()->InboundBranches().Length());
    EXPECT_EQ(3u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    if true [t: %fn3, f: %fn4, m: %fn5]
      # True block
      %fn3 = block {
        br %fn5
      }

      # False block
      %fn4 = block {
        br %fn5
      }

    # Merge block
    %fn5 = block {
      br %fn6  # return
    }

  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, IfStatement_TrueReturns) {
    auto* ast_if = If(true, Block(Return()));
    WrapInFunction(ast_if);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(1u, flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(2u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    if true [t: %fn3, f: %fn4, m: %fn5]
      # True block
      %fn3 = block {
        br %fn6  # return
      }
      # False block
      %fn4 = block {
        br %fn5
      }

    # Merge block
    %fn5 = block {
      br %fn6  # return
    }

  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, IfStatement_FalseReturns) {
    auto* ast_if = If(true, Block(), Else(Block(Return())));
    WrapInFunction(ast_if);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(1u, flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(2u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    if true [t: %fn3, f: %fn4, m: %fn5]
      # True block
      %fn3 = block {
        br %fn5
      }

      # False block
      %fn4 = block {
        br %fn6  # return
      }
    # Merge block
    %fn5 = block {
      br %fn6  # return
    }

  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, IfStatement_BothReturn) {
    auto* ast_if = If(true, Block(Return()), Else(Block(Return())));
    WrapInFunction(ast_if);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(1u, flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->False()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(2u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    if true [t: %fn3, f: %fn4]
      # True block
      %fn3 = block {
        br %fn5  # return
      }
      # False block
      %fn4 = block {
        br %fn5  # return
      }

  }

  %fn5 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, IfStatement_JumpChainToMerge) {
    auto* ast_loop = Loop(Block(Break()));
    auto* ast_if = If(true, Block(ast_loop));
    WrapInFunction(ast_if);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* if_flow = FindSingleValue<ir::If>(m);
    ASSERT_NE(if_flow, nullptr);

    auto* loop_flow = FindSingleValue<ir::Loop>(m);
    ASSERT_NE(loop_flow, nullptr);

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    if true [t: %fn3, f: %fn4, m: %fn5]
      # True block
      %fn3 = block {
        loop [s: %fn6, c: %fn7, m: %fn8]
          %fn6 = block {
            br %fn8
          }

          # Continuing block
          %fn7 = block {
            br %fn6
          }

        # Merge block
        %fn8 = block {
          br %fn5
        }


      }

      # False block
      %fn4 = block {
        br %fn5
      }

    # Merge block
    %fn5 = block {
      br %fn9  # return
    }

  }

  %fn9 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithBreak) {
    auto* ast_loop = Loop(Block(Break()));
    WrapInFunction(ast_loop);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Loop>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, flow->Start()->InboundBranches().Length());
    EXPECT_EQ(0u, flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        br %fn5
      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn6  # return
    }

  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithContinue) {
    auto* ast_if = If(true, Block(Break()));
    auto* ast_loop = Loop(Block(ast_if, Continue()));
    WrapInFunction(ast_loop);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* loop_flow = FindSingleValue<ir::Loop>(m);

    auto* if_flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, loop_flow->Start()->InboundBranches().Length());
    EXPECT_EQ(1u, loop_flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, loop_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, if_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        if true [t: %fn6, f: %fn7, m: %fn8]
          # True block
          %fn6 = block {
            br %fn5
          }

          # False block
          %fn7 = block {
            br %fn8
          }

        # Merge block
        %fn8 = block {
          br %fn4
        }


      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn9  # return
    }

  }

  %fn9 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithContinuing_BreakIf) {
    auto* ast_break_if = BreakIf(true);
    auto* ast_loop = Loop(Block(), Block(ast_break_if));
    WrapInFunction(ast_loop);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* loop_flow = FindSingleValue<ir::Loop>(m);
    auto* break_if_flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, loop_flow->Start()->InboundBranches().Length());
    EXPECT_EQ(1u, loop_flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, loop_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, break_if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, break_if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, break_if_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        br %fn4
      }

      # Continuing block
      %fn4 = block {
        if true [t: %fn6, f: %fn7, m: %fn8]
          # True block
          %fn6 = block {
            br %fn5
          }

          # False block
          %fn7 = block {
            br %fn8
          }

        # Merge block
        %fn8 = block {
          br %fn3
        }


      }

    # Merge block
    %fn5 = block {
      br %fn9  # return
    }

  }

  %fn9 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_Continuing_Body_Scope) {
    auto* a = Decl(Let("a", Expr(true)));
    auto* ast_break_if = BreakIf("a");
    auto* ast_loop = Loop(Block(a), Block(ast_break_if));
    WrapInFunction(ast_loop);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        br %fn4
      }

      # Continuing block
      %fn4 = block {
        if true [t: %fn6, f: %fn7, m: %fn8]
          # True block
          %fn6 = block {
            br %fn5
          }

          # False block
          %fn7 = block {
            br %fn8
          }

        # Merge block
        %fn8 = block {
          br %fn3
        }


      }

    # Merge block
    %fn5 = block {
      br %fn9  # return
    }

  }

  %fn9 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithReturn) {
    auto* ast_if = If(true, Block(Return()));
    auto* ast_loop = Loop(Block(ast_if, Continue()));
    WrapInFunction(ast_loop);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* loop_flow = FindSingleValue<ir::Loop>(m);
    auto* if_flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, loop_flow->Start()->InboundBranches().Length());
    EXPECT_EQ(1u, loop_flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(0u, loop_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, if_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4]
      %fn3 = block {
        if true [t: %fn5, f: %fn6, m: %fn7]
          # True block
          %fn5 = block {
            br %fn8  # return
          }
          # False block
          %fn6 = block {
            br %fn7
          }

        # Merge block
        %fn7 = block {
          br %fn4
        }


      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }


  }

  %fn8 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithOnlyReturn) {
    auto* ast_loop = Loop(Block(Return(), Continue()));
    WrapInFunction(ast_loop, If(true, Block(Return())));

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* loop_flow = FindSingleValue<ir::Loop>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, loop_flow->Start()->InboundBranches().Length());
    EXPECT_EQ(0u, loop_flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(0u, loop_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4]
      %fn3 = block {
        br %fn5  # return
      }
      # Continuing block
      %fn4 = block {
        br %fn3
      }


  }

  %fn5 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithOnlyReturn_ContinuingBreakIf) {
    // Note, even though there is code in the loop merge (specifically, the
    // `ast_if` below), it doesn't get emitted as there is no way to reach the
    // loop merge due to the loop itself doing a `return`. This is why the
    // loop merge gets marked as Dead and the `ast_if` doesn't appear.
    //
    // Similar, the continuing block goes away as there is no way to get there, so it's treated
    // as dead code and dropped.
    auto* ast_break_if = BreakIf(true);
    auto* ast_loop = Loop(Block(Return()), Block(ast_break_if));
    auto* ast_if = If(true, Block(Return()));
    WrapInFunction(Block(ast_loop, ast_if));

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* loop_flow = FindSingleValue<ir::Loop>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, loop_flow->Start()->InboundBranches().Length());
    EXPECT_EQ(0u, loop_flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, loop_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(3u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        br %fn6  # return
      }
      # Continuing block
      %fn4 = block {
        if true [t: %fn7, f: %fn8, m: %fn9]
          # True block
          %fn7 = block {
            br %fn5
          }

          # False block
          %fn8 = block {
            br %fn9
          }

        # Merge block
        %fn9 = block {
          br %fn3
        }


      }

    # Merge block
    %fn5 = block {
      if true [t: %fn10, f: %fn11, m: %fn12]
        # True block
        %fn10 = block {
          br %fn6  # return
        }
        # False block
        %fn11 = block {
          br %fn12
        }

      # Merge block
      %fn12 = block {
        br %fn6  # return
      }

    }


  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_WithIf_BothBranchesBreak) {
    auto* ast_if = If(true, Block(Break()), Else(Block(Break())));
    auto* ast_loop = Loop(Block(ast_if, Continue()));
    WrapInFunction(ast_loop);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* loop_flow = FindSingleValue<ir::Loop>(m);
    auto* if_flow = FindSingleValue<ir::If>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, loop_flow->Start()->InboundBranches().Length());
    EXPECT_EQ(0u, loop_flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(2u, loop_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        if true [t: %fn6, f: %fn7]
          # True block
          %fn6 = block {
            br %fn5
          }

          # False block
          %fn7 = block {
            br %fn5
          }


      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn8  # return
    }

  }

  %fn8 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Loop_Nested) {
    auto* ast_if_a = If(true, Block(Break()));
    auto* ast_if_b = If(true, Block(Continue()));
    auto* ast_if_c = BreakIf(true);
    auto* ast_if_d = If(true, Block(Break()));

    auto* ast_loop_d = Loop(Block(), Block(ast_if_c));
    auto* ast_loop_c = Loop(Block(Break()));

    auto* ast_loop_b = Loop(Block(ast_if_a, ast_if_b), Block(ast_loop_c, ast_loop_d));
    auto* ast_loop_a = Loop(Block(ast_loop_b, ast_if_d));

    WrapInFunction(ast_loop_a);

    auto m = Build();
    ASSERT_TRUE(m) << (!m ? m.Failure() : "");

    EXPECT_EQ(Disassemble(m.Get()),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        loop [s: %fn6, c: %fn7, m: %fn8]
          %fn6 = block {
            if true [t: %fn9, f: %fn10, m: %fn11]
              # True block
              %fn9 = block {
                br %fn8
              }

              # False block
              %fn10 = block {
                br %fn11
              }

            # Merge block
            %fn11 = block {
              if true [t: %fn12, f: %fn13, m: %fn14]
                # True block
                %fn12 = block {
                  br %fn7
                }

                # False block
                %fn13 = block {
                  br %fn14
                }

              # Merge block
              %fn14 = block {
                br %fn7
              }


            }


          }

          # Continuing block
          %fn7 = block {
            loop [s: %fn15, c: %fn16, m: %fn17]
              %fn15 = block {
                br %fn17
              }

              # Continuing block
              %fn16 = block {
                br %fn15
              }

            # Merge block
            %fn17 = block {
              loop [s: %fn18, c: %fn19, m: %fn20]
                %fn18 = block {
                  br %fn19
                }

                # Continuing block
                %fn19 = block {
                  if true [t: %fn21, f: %fn22, m: %fn23]
                    # True block
                    %fn21 = block {
                      br %fn20
                    }

                    # False block
                    %fn22 = block {
                      br %fn23
                    }

                  # Merge block
                  %fn23 = block {
                    br %fn18
                  }


                }

              # Merge block
              %fn20 = block {
                br %fn6
              }


            }


          }

        # Merge block
        %fn8 = block {
          if true [t: %fn24, f: %fn25, m: %fn26]
            # True block
            %fn24 = block {
              br %fn5
            }

            # False block
            %fn25 = block {
              br %fn26
            }

          # Merge block
          %fn26 = block {
            br %fn4
          }


        }


      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn27  # return
    }

  }

  %fn27 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, While) {
    auto* ast_while = While(false, Block());
    WrapInFunction(ast_while);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Loop>(m);

    ASSERT_NE(flow->Start()->Branch(), nullptr);
    ASSERT_TRUE(flow->Start()->Branch()->Is<ir::If>());
    auto* if_flow = flow->Start()->Branch()->As<ir::If>();

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Start()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, if_flow->Merge()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        if false [t: %fn6, f: %fn7, m: %fn8]
          # True block
          %fn6 = block {
            br %fn8
          }

          # False block
          %fn7 = block {
            br %fn5
          }

        # Merge block
        %fn8 = block {
          br %fn4
        }


      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn9  # return
    }

  }

  %fn9 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, While_Return) {
    auto* ast_while = While(true, Block(Return()));
    WrapInFunction(ast_while);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Loop>(m);

    ASSERT_NE(flow->Start()->Branch(), nullptr);
    ASSERT_TRUE(flow->Start()->Branch()->Is<ir::If>());
    auto* if_flow = flow->Start()->Branch()->As<ir::If>();

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, func->EndTarget()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Start()->InboundBranches().Length());
    EXPECT_EQ(0u, flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, if_flow->Merge()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        if true [t: %fn6, f: %fn7, m: %fn8]
          # True block
          %fn6 = block {
            br %fn8
          }

          # False block
          %fn7 = block {
            br %fn5
          }

        # Merge block
        %fn8 = block {
          br %fn9  # return
        }

      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn9  # return
    }

  }

  %fn9 = func_terminator
}
)");
}

// TODO(dsinclair): Enable when variable declarations and increment are supported
TEST_F(IR_BuilderImplTest, DISABLED_For) {
    // for(var i: 0; i < 10; i++) {
    // }
    //
    // func -> loop -> loop start -> if true
    //                            -> if false
    //
    //   [if true] -> if merge
    //   [if false] -> loop merge
    //   [if merge] -> loop continuing
    //   [loop continuing] -> loop start
    //   [loop merge] -> func end
    //
    auto* ast_for = For(Decl(Var("i", ty.i32())), LessThan("i", 10_a), Increment("i"), Block());
    WrapInFunction(ast_for);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Loop>(m);

    ASSERT_NE(flow->Start()->Branch(), nullptr);
    ASSERT_TRUE(flow->Start()->Branch()->Is<ir::If>());
    auto* if_flow = flow->Start()->Branch()->As<ir::If>();

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Start()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->True()->InboundBranches().Length());
    EXPECT_EQ(1u, if_flow->False()->InboundBranches().Length());
    EXPECT_EQ(2u, if_flow->Merge()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m), R"()");
}

TEST_F(IR_BuilderImplTest, For_NoInitCondOrContinuing) {
    auto* ast_for = For(nullptr, nullptr, nullptr, Block(Break()));
    WrapInFunction(ast_for);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Loop>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    EXPECT_EQ(2u, flow->Start()->InboundBranches().Length());
    EXPECT_EQ(0u, flow->Continuing()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    loop [s: %fn3, c: %fn4, m: %fn5]
      %fn3 = block {
        br %fn5
      }

      # Continuing block
      %fn4 = block {
        br %fn3
      }

    # Merge block
    %fn5 = block {
      br %fn6  # return
    }

  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Switch) {
    auto* ast_switch = Switch(
        1_i, utils::Vector{Case(utils::Vector{CaseSelector(0_i)}, Block()),
                           Case(utils::Vector{CaseSelector(1_i)}, Block()), DefaultCase(Block())});

    WrapInFunction(ast_switch);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Switch>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    auto cases = flow->Cases();
    ASSERT_EQ(3u, cases.Length());

    ASSERT_EQ(1u, cases[0].selectors.Length());
    ASSERT_TRUE(cases[0].selectors[0].val->Value()->Is<constant::Scalar<tint::i32>>());
    EXPECT_EQ(0_i,
              cases[0].selectors[0].val->Value()->As<constant::Scalar<tint::i32>>()->ValueOf());

    ASSERT_EQ(1u, cases[1].selectors.Length());
    ASSERT_TRUE(cases[1].selectors[0].val->Value()->Is<constant::Scalar<tint::i32>>());
    EXPECT_EQ(1_i,
              cases[1].selectors[0].val->Value()->As<constant::Scalar<tint::i32>>()->ValueOf());

    ASSERT_EQ(1u, cases[2].selectors.Length());
    EXPECT_TRUE(cases[2].selectors[0].IsDefault());

    EXPECT_EQ(1u, cases[0].Start()->InboundBranches().Length());
    EXPECT_EQ(1u, cases[1].Start()->InboundBranches().Length());
    EXPECT_EQ(1u, cases[2].Start()->InboundBranches().Length());
    EXPECT_EQ(4u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    switch 1i [c: (0i, %fn3), c: (1i, %fn4), c: (default, %fn5), m: %fn6]
      # Case block
      %fn3 = block {
        br %fn6
      }

      # Case block
      %fn4 = block {
        br %fn6
      }

      # Case block
      %fn5 = block {
        br %fn6
      }

    # Merge block
    %fn6 = block {
      br %fn7  # return
    }

  }

  %fn7 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Switch_MultiSelector) {
    auto* ast_switch = Switch(
        1_i,
        utils::Vector{Case(
            utils::Vector{CaseSelector(0_i), CaseSelector(1_i), DefaultCaseSelector()}, Block())});

    WrapInFunction(ast_switch);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Switch>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    auto cases = flow->Cases();
    ASSERT_EQ(1u, cases.Length());
    ASSERT_EQ(3u, cases[0].selectors.Length());
    ASSERT_TRUE(cases[0].selectors[0].val->Value()->Is<constant::Scalar<tint::i32>>());
    EXPECT_EQ(0_i,
              cases[0].selectors[0].val->Value()->As<constant::Scalar<tint::i32>>()->ValueOf());

    ASSERT_TRUE(cases[0].selectors[1].val->Value()->Is<constant::Scalar<tint::i32>>());
    EXPECT_EQ(1_i,
              cases[0].selectors[1].val->Value()->As<constant::Scalar<tint::i32>>()->ValueOf());

    EXPECT_TRUE(cases[0].selectors[2].IsDefault());

    EXPECT_EQ(1u, cases[0].Start()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    switch 1i [c: (0i 1i default, %fn3), m: %fn4]
      # Case block
      %fn3 = block {
        br %fn4
      }

    # Merge block
    %fn4 = block {
      br %fn5  # return
    }

  }

  %fn5 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Switch_OnlyDefault) {
    auto* ast_switch = Switch(1_i, utils::Vector{DefaultCase(Block())});
    WrapInFunction(ast_switch);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Switch>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    auto cases = flow->Cases();
    ASSERT_EQ(1u, cases.Length());
    ASSERT_EQ(1u, cases[0].selectors.Length());
    EXPECT_TRUE(cases[0].selectors[0].IsDefault());

    EXPECT_EQ(1u, cases[0].Start()->InboundBranches().Length());
    EXPECT_EQ(2u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    switch 1i [c: (default, %fn3), m: %fn4]
      # Case block
      %fn3 = block {
        br %fn4
      }

    # Merge block
    %fn4 = block {
      br %fn5  # return
    }

  }

  %fn5 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Switch_WithBreak) {
    auto* ast_switch = Switch(1_i, utils::Vector{Case(utils::Vector{CaseSelector(0_i)},
                                                      Block(Break(), If(true, Block(Return())))),
                                                 DefaultCase(Block())});
    WrapInFunction(ast_switch);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    auto* flow = FindSingleValue<ir::Switch>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    auto cases = flow->Cases();
    ASSERT_EQ(2u, cases.Length());
    ASSERT_EQ(1u, cases[0].selectors.Length());
    ASSERT_TRUE(cases[0].selectors[0].val->Value()->Is<constant::Scalar<tint::i32>>());
    EXPECT_EQ(0_i,
              cases[0].selectors[0].val->Value()->As<constant::Scalar<tint::i32>>()->ValueOf());

    ASSERT_EQ(1u, cases[1].selectors.Length());
    EXPECT_TRUE(cases[1].selectors[0].IsDefault());

    EXPECT_EQ(1u, cases[0].Start()->InboundBranches().Length());
    EXPECT_EQ(1u, cases[1].Start()->InboundBranches().Length());
    EXPECT_EQ(3u, flow->Merge()->InboundBranches().Length());
    // This is 1 because the if is dead-code eliminated and the return doesn't happen.
    EXPECT_EQ(1u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    switch 1i [c: (0i, %fn3), c: (default, %fn4), m: %fn5]
      # Case block
      %fn3 = block {
        br %fn5
      }

      # Case block
      %fn4 = block {
        br %fn5
      }

    # Merge block
    %fn5 = block {
      br %fn6  # return
    }

  }

  %fn6 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Switch_AllReturn) {
    auto* ast_switch =
        Switch(1_i, utils::Vector{Case(utils::Vector{CaseSelector(0_i)}, Block(Return())),
                                  DefaultCase(Block(Return()))});
    auto* ast_if = If(true, Block(Return()));
    WrapInFunction(ast_switch, ast_if);

    auto res = Build();
    ASSERT_TRUE(res) << (!res ? res.Failure() : "");

    auto m = res.Move();
    ASSERT_EQ(FindSingleValue<ir::If>(m), nullptr);

    auto* flow = FindSingleValue<ir::Switch>(m);

    ASSERT_EQ(1u, m.functions.Length());
    auto* func = m.functions[0];

    auto cases = flow->Cases();
    ASSERT_EQ(2u, cases.Length());
    ASSERT_EQ(1u, cases[0].selectors.Length());
    ASSERT_TRUE(cases[0].selectors[0].val->Value()->Is<constant::Scalar<tint::i32>>());
    EXPECT_EQ(0_i,
              cases[0].selectors[0].val->Value()->As<constant::Scalar<tint::i32>>()->ValueOf());

    ASSERT_EQ(1u, cases[1].selectors.Length());
    EXPECT_TRUE(cases[1].selectors[0].IsDefault());

    EXPECT_EQ(1u, cases[0].Start()->InboundBranches().Length());
    EXPECT_EQ(1u, cases[1].Start()->InboundBranches().Length());
    EXPECT_EQ(1u, flow->Merge()->InboundBranches().Length());
    EXPECT_EQ(2u, func->EndTarget()->InboundBranches().Length());

    EXPECT_EQ(Disassemble(m),
              R"(%fn1 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn2 {
  %fn2 = block {
    switch 1i [c: (0i, %fn3), c: (default, %fn4)]
      # Case block
      %fn3 = block {
        br %fn5  # return
      }
      # Case block
      %fn4 = block {
        br %fn5  # return
      }

  }

  %fn5 = func_terminator
}
)");
}

TEST_F(IR_BuilderImplTest, Emit_Phony) {
    Func("b", utils::Empty, ty.i32(), Return(1_i));
    WrapInFunction(Ignore(Call("b")));

    auto m = Build();
    ASSERT_TRUE(m) << (!m ? m.Failure() : "");

    EXPECT_EQ(Disassemble(m.Get()),
              R"(%fn1 = func b():i32 -> %fn2 {
  %fn2 = block {
    br %fn3 1i  # return
  }
  %fn3 = func_terminator
}
%fn4 = func test_function():void [@compute @workgroup_size(1, 1, 1)] -> %fn5 {
  %fn5 = block {
    %1:i32 = call b
    br %fn6  # return
  }
  %fn6 = func_terminator
}
)");
}

}  // namespace
}  // namespace tint::ir
