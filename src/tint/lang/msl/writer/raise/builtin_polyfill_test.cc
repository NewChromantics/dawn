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

#include "src/tint/lang/msl/writer/raise/builtin_polyfill.h"

#include <utility>

#include "gtest/gtest.h"
#include "src/tint/lang/core/access.h"
#include "src/tint/lang/core/fluent_types.h"
#include "src/tint/lang/core/ir/transform/helper_test.h"
#include "src/tint/lang/core/number.h"
#include "src/tint/lang/core/texel_format.h"
#include "src/tint/lang/core/type/atomic.h"
#include "src/tint/lang/core/type/builtin_structs.h"
#include "src/tint/lang/core/type/depth_texture.h"
#include "src/tint/lang/core/type/multisampled_texture.h"
#include "src/tint/lang/core/type/sampled_texture.h"
#include "src/tint/lang/core/type/storage_texture.h"
#include "src/tint/lang/core/type/texture_dimension.h"

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

namespace tint::msl::writer::raise {
namespace {

using MslWriter_BuiltinPolyfillTest = core::ir::transform::TransformTest;

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicAdd_Workgroup_I32) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicAdd, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicAdd %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_add_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicAdd_Storage_U32) {
    auto* a = b.FunctionParam<ptr<storage, atomic<u32>>>("a");
    auto* func = b.Function("foo", ty.u32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<u32>(core::BuiltinFn::kAtomicAdd, a, 1_u);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<storage, atomic<u32>, read_write>):u32 {
  $B1: {
    %3:u32 = atomicAdd %a, 1u
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<storage, atomic<u32>, read_write>):u32 {
  $B1: {
    %3:u32 = msl.atomic_fetch_add_explicit %a, 1u, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicAnd) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicAnd, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicAnd %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_and_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicExchange) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicExchange, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicExchange %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_exchange_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicLoad) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicLoad, a);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicLoad %a
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_load_explicit %a, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicMax) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicMax, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicMax %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_max_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicMin) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicMin, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicMin %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_min_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicOr) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicOr, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicOr %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_or_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicStore) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kAtomicStore, a, 1_i);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):void {
  $B1: {
    %3:void = atomicStore %a, 1i
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):void {
  $B1: {
    %3:void = msl.atomic_store_explicit %a, 1i, 0u
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicSub) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicSub, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicSub %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_sub_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicXor) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<i32>(core::BuiltinFn::kAtomicXor, a, 1_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = atomicXor %a, 1i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:i32 = msl.atomic_fetch_xor_explicit %a, 1i, 0u
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, AtomicCompareExchangeWeak) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.i32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, a, 1_i, 2_i);
        auto* if_ = b.If(b.Access<bool>(result, 1_u));
        b.Append(if_->True(), [&] {  //
            b.Return(func, b.Access<i32>(result, 0_u));
        });
        b.Return(func, 42_i);
    });

    auto* src = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:__atomic_compare_exchange_result_i32 = atomicCompareExchangeWeak %a, 1i, 2i
    %4:bool = access %3, 1u
    if %4 [t: $B2] {  # if_1
      $B2: {  # true
        %5:i32 = access %3, 0u
        ret %5
      }
    }
    ret 42i
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:__atomic_compare_exchange_result_i32 = call %4, %a, 1i, 2i
    %5:bool = access %3, 1u
    if %5 [t: $B2] {  # if_1
      $B2: {  # true
        %6:i32 = access %3, 0u
        ret %6
      }
    }
    ret 42i
  }
}
%4 = func(%atomic_ptr:ptr<workgroup, atomic<i32>, read_write>, %cmp:i32, %val:i32):__atomic_compare_exchange_result_i32 {
  $B3: {
    %old_value:ptr<function, i32, read_write> = var, %cmp
    %11:bool = msl.atomic_compare_exchange_weak_explicit %atomic_ptr, %old_value, %val, 0u, 0u
    %12:i32 = load %old_value
    %13:__atomic_compare_exchange_result_i32 = construct %12, %11
    ret %13
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest,
       AtomicCompareExchangeWeak_Multiple_SameAddressSpace_SameType) {
    auto* a = b.FunctionParam<ptr<workgroup, atomic<i32>>>("a");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({a});
    b.Append(func->Block(), [&] {
        auto* result_a =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.i32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, a, 1_i, 2_i);
        auto* result_b =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.i32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, a, 3_i, 4_i);
        auto* if_ = b.If(b.Access<bool>(result_a, 1_u));
        b.Append(if_->True(), [&] {  //
            b.Return(func, b.Access<i32>(result_a, 0_u));
        });
        b.Return(func, b.Access<i32>(result_b, 0_u));
    });

    auto* src = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:__atomic_compare_exchange_result_i32 = atomicCompareExchangeWeak %a, 1i, 2i
    %4:__atomic_compare_exchange_result_i32 = atomicCompareExchangeWeak %a, 3i, 4i
    %5:bool = access %3, 1u
    if %5 [t: $B2] {  # if_1
      $B2: {  # true
        %6:i32 = access %3, 0u
        ret %6
      }
    }
    %7:i32 = access %4, 0u
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%a:ptr<workgroup, atomic<i32>, read_write>):i32 {
  $B1: {
    %3:__atomic_compare_exchange_result_i32 = call %4, %a, 1i, 2i
    %5:__atomic_compare_exchange_result_i32 = call %4, %a, 3i, 4i
    %6:bool = access %3, 1u
    if %6 [t: $B2] {  # if_1
      $B2: {  # true
        %7:i32 = access %3, 0u
        ret %7
      }
    }
    %8:i32 = access %5, 0u
    ret %8
  }
}
%4 = func(%atomic_ptr:ptr<workgroup, atomic<i32>, read_write>, %cmp:i32, %val:i32):__atomic_compare_exchange_result_i32 {
  $B3: {
    %old_value:ptr<function, i32, read_write> = var, %cmp
    %13:bool = msl.atomic_compare_exchange_weak_explicit %atomic_ptr, %old_value, %val, 0u, 0u
    %14:i32 = load %old_value
    %15:__atomic_compare_exchange_result_i32 = construct %14, %13
    ret %15
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest,
       AtomicCompareExchangeWeak_Multiple_SameAddressSpace_DifferentType) {
    auto* ai = b.FunctionParam<ptr<workgroup, atomic<i32>>>("ai");
    auto* au = b.FunctionParam<ptr<workgroup, atomic<u32>>>("au");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({ai, au});
    b.Append(func->Block(), [&] {
        auto* result_a =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.i32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, ai, 1_i, 2_i);
        auto* result_b =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.u32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, au, 3_u, 4_u);
        auto* if_ = b.If(b.Access<bool>(result_a, 1_u));
        b.Append(if_->True(), [&] {  //
            b.Return(func, b.Access<i32>(result_a, 0_u));
        });
        b.Return(func, b.Convert<i32>(b.Access<u32>(result_b, 0_u)));
    });

    auto* src = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

__atomic_compare_exchange_result_u32 = struct @align(4) {
  old_value:u32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%ai:ptr<workgroup, atomic<i32>, read_write>, %au:ptr<workgroup, atomic<u32>, read_write>):i32 {
  $B1: {
    %4:__atomic_compare_exchange_result_i32 = atomicCompareExchangeWeak %ai, 1i, 2i
    %5:__atomic_compare_exchange_result_u32 = atomicCompareExchangeWeak %au, 3u, 4u
    %6:bool = access %4, 1u
    if %6 [t: $B2] {  # if_1
      $B2: {  # true
        %7:i32 = access %4, 0u
        ret %7
      }
    }
    %8:u32 = access %5, 0u
    %9:i32 = convert %8
    ret %9
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

__atomic_compare_exchange_result_u32 = struct @align(4) {
  old_value:u32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%ai:ptr<workgroup, atomic<i32>, read_write>, %au:ptr<workgroup, atomic<u32>, read_write>):i32 {
  $B1: {
    %4:__atomic_compare_exchange_result_i32 = call %5, %ai, 1i, 2i
    %6:__atomic_compare_exchange_result_u32 = call %7, %au, 3u, 4u
    %8:bool = access %4, 1u
    if %8 [t: $B2] {  # if_1
      $B2: {  # true
        %9:i32 = access %4, 0u
        ret %9
      }
    }
    %10:u32 = access %6, 0u
    %11:i32 = convert %10
    ret %11
  }
}
%5 = func(%atomic_ptr:ptr<workgroup, atomic<i32>, read_write>, %cmp:i32, %val:i32):__atomic_compare_exchange_result_i32 {
  $B3: {
    %old_value:ptr<function, i32, read_write> = var, %cmp
    %16:bool = msl.atomic_compare_exchange_weak_explicit %atomic_ptr, %old_value, %val, 0u, 0u
    %17:i32 = load %old_value
    %18:__atomic_compare_exchange_result_i32 = construct %17, %16
    ret %18
  }
}
%7 = func(%atomic_ptr_1:ptr<workgroup, atomic<u32>, read_write>, %cmp_1:u32, %val_1:u32):__atomic_compare_exchange_result_u32 {  # %atomic_ptr_1: 'atomic_ptr', %cmp_1: 'cmp', %val_1: 'val'
  $B4: {
    %old_value_1:ptr<function, u32, read_write> = var, %cmp_1  # %old_value_1: 'old_value'
    %23:bool = msl.atomic_compare_exchange_weak_explicit %atomic_ptr_1, %old_value_1, %val_1, 0u, 0u
    %24:u32 = load %old_value_1
    %25:__atomic_compare_exchange_result_u32 = construct %24, %23
    ret %25
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest,
       AtomicCompareExchangeWeak_Multiple_DifferentAddressSpace_SameType) {
    auto* aw = b.FunctionParam<ptr<workgroup, atomic<i32>>>("aw");
    auto* as = b.FunctionParam<ptr<storage, atomic<i32>>>("as");
    auto* func = b.Function("foo", ty.i32());
    func->SetParams({aw, as});
    b.Append(func->Block(), [&] {
        auto* result_a =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.i32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, aw, 1_i, 2_i);
        auto* result_b =
            b.Call(core::type::CreateAtomicCompareExchangeResult(ty, mod.symbols, ty.i32()),
                   core::BuiltinFn::kAtomicCompareExchangeWeak, as, 3_i, 4_i);
        auto* if_ = b.If(b.Access<bool>(result_a, 1_u));
        b.Append(if_->True(), [&] {  //
            b.Return(func, b.Access<i32>(result_a, 0_u));
        });
        b.Return(func, b.Access<i32>(result_b, 0_u));
    });

    auto* src = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%aw:ptr<workgroup, atomic<i32>, read_write>, %as:ptr<storage, atomic<i32>, read_write>):i32 {
  $B1: {
    %4:__atomic_compare_exchange_result_i32 = atomicCompareExchangeWeak %aw, 1i, 2i
    %5:__atomic_compare_exchange_result_i32 = atomicCompareExchangeWeak %as, 3i, 4i
    %6:bool = access %4, 1u
    if %6 [t: $B2] {  # if_1
      $B2: {  # true
        %7:i32 = access %4, 0u
        ret %7
      }
    }
    %8:i32 = access %5, 0u
    ret %8
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
__atomic_compare_exchange_result_i32 = struct @align(4) {
  old_value:i32 @offset(0)
  exchanged:bool @offset(4)
}

%foo = func(%aw:ptr<workgroup, atomic<i32>, read_write>, %as:ptr<storage, atomic<i32>, read_write>):i32 {
  $B1: {
    %4:__atomic_compare_exchange_result_i32 = call %5, %aw, 1i, 2i
    %6:__atomic_compare_exchange_result_i32 = call %7, %as, 3i, 4i
    %8:bool = access %4, 1u
    if %8 [t: $B2] {  # if_1
      $B2: {  # true
        %9:i32 = access %4, 0u
        ret %9
      }
    }
    %10:i32 = access %6, 0u
    ret %10
  }
}
%5 = func(%atomic_ptr:ptr<workgroup, atomic<i32>, read_write>, %cmp:i32, %val:i32):__atomic_compare_exchange_result_i32 {
  $B3: {
    %old_value:ptr<function, i32, read_write> = var, %cmp
    %15:bool = msl.atomic_compare_exchange_weak_explicit %atomic_ptr, %old_value, %val, 0u, 0u
    %16:i32 = load %old_value
    %17:__atomic_compare_exchange_result_i32 = construct %16, %15
    ret %17
  }
}
%7 = func(%atomic_ptr_1:ptr<storage, atomic<i32>, read_write>, %cmp_1:i32, %val_1:i32):__atomic_compare_exchange_result_i32 {  # %atomic_ptr_1: 'atomic_ptr', %cmp_1: 'cmp', %val_1: 'val'
  $B4: {
    %old_value_1:ptr<function, i32, read_write> = var, %cmp_1  # %old_value_1: 'old_value'
    %22:bool = msl.atomic_compare_exchange_weak_explicit %atomic_ptr_1, %old_value_1, %val_1, 0u, 0u
    %23:i32 = load %old_value_1
    %24:__atomic_compare_exchange_result_i32 = construct %23, %22
    ret %24
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureDimensions_1d) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k1d, ty.f32()));
    auto* func = b.Function("foo", ty.u32());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<u32>(core::BuiltinFn::kTextureDimensions, t);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_1d<f32>):u32 {
  $B1: {
    %3:u32 = textureDimensions %t
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_1d<f32>):u32 {
  $B1: {
    %3:u32 = %t.get_width
    %4:u32 = construct %3
    ret %4
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureDimensions_2d_WithoutLod) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* func = b.Function("foo", ty.vec2<u32>());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, t);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>):vec2<u32> {
  $B1: {
    %3:vec2<u32> = textureDimensions %t
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>):vec2<u32> {
  $B1: {
    %3:u32 = %t.get_width 0u
    %4:u32 = %t.get_height 0u
    %5:vec2<u32> = construct %3, %4
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureDimensions_2d_WithI32Lod) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* func = b.Function("foo", ty.vec2<u32>());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, t, 3_i);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>):vec2<u32> {
  $B1: {
    %3:vec2<u32> = textureDimensions %t, 3i
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>):vec2<u32> {
  $B1: {
    %3:u32 = convert 3i
    %4:u32 = %t.get_width %3
    %5:u32 = %t.get_height %3
    %6:vec2<u32> = construct %4, %5
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureDimensions_3d) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k3d, ty.f32()));
    auto* func = b.Function("foo", ty.vec3<u32>());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec3<u32>>(core::BuiltinFn::kTextureDimensions, t);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_3d<f32>):vec3<u32> {
  $B1: {
    %3:vec3<u32> = textureDimensions %t
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_3d<f32>):vec3<u32> {
  $B1: {
    %3:u32 = %t.get_width 0u
    %4:u32 = %t.get_height 0u
    %5:u32 = %t.get_depth 0u
    %6:vec3<u32> = construct %3, %4, %5
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureGather_2d_UnsignedComponent) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, 1_u, t, s, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = textureGather 1u, %t, %s, %coords
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = %t.gather %s, %coords, vec2<i32>(0i), 1u
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureGather_2d_SignedComponent) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, 2_i, t, s, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = textureGather 2i, %t, %s, %coords
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = %t.gather %s, %coords, vec2<i32>(0i), 2u
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureGather_2d_WithOffset) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* offset = b.FunctionParam("offset", ty.vec2<i32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, offset});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, 1_u, t, s, coords, offset);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %offset:vec2<i32>):vec4<f32> {
  $B1: {
    %6:vec4<f32> = textureGather 1u, %t, %s, %coords, %offset
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %offset:vec2<i32>):vec4<f32> {
  $B1: {
    %6:vec4<f32> = %t.gather %s, %coords, %offset, 1u
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureGather_Depth2d) {
    auto* t =
        b.FunctionParam("t", ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, t, s, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_depth_2d, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = textureGather %t, %s, %coords
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_depth_2d, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = %t.gather %s, %coords, vec2<i32>(0i)
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_1d_U32Coord) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k1d, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.u32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, read>, %coords:u32):vec4<f32> {
  $B1: {
    %4:vec4<f32> = textureLoad %t, %coords
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, read>, %coords:u32):vec4<f32> {
  $B1: {
    %4:vec4<f32> = %t.read %coords
    ret %4
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_1d_I32Coord) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k1d, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.i32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, read>, %coords:i32):vec4<f32> {
  $B1: {
    %4:vec4<f32> = textureLoad %t, %coords
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, read>, %coords:i32):vec4<f32> {
  $B1: {
    %4:u32 = convert %coords
    %5:vec4<f32> = %t.read %4
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_2d_U32Coords) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<u32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, read>, %coords:vec2<u32>):vec4<f32> {
  $B1: {
    %4:vec4<f32> = textureLoad %t, %coords
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, read>, %coords:vec2<u32>):vec4<f32> {
  $B1: {
    %4:vec4<f32> = %t.read %coords
    ret %4
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_2d_I32Coords) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, read>, %coords:vec2<i32>):vec4<f32> {
  $B1: {
    %4:vec4<f32> = textureLoad %t, %coords
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, read>, %coords:vec2<i32>):vec4<f32> {
  $B1: {
    %4:vec2<u32> = convert %coords
    %5:vec4<f32> = %t.read %4
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_2d_WithLevel) {
    auto* texture_ty =
        ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.i32());
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* level = b.FunctionParam("level", ty.i32());
    auto* func = b.Function("foo", ty.vec4<i32>());
    func->SetParams({t, coords, level});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<i32>>(core::BuiltinFn::kTextureLoad, t, coords, level);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<i32>, %coords:vec2<i32>, %level:i32):vec4<i32> {
  $B1: {
    %5:vec4<i32> = textureLoad %t, %coords, %level
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<i32>, %coords:vec2<i32>, %level:i32):vec4<i32> {
  $B1: {
    %5:vec2<u32> = convert %coords
    %6:vec4<i32> = %t.read %5, %level
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_2darray_U32Index) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2dArray, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* index = b.FunctionParam("index", ty.u32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords, index});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords, index);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, read>, %coords:vec2<i32>, %index:u32):vec4<f32> {
  $B1: {
    %5:vec4<f32> = textureLoad %t, %coords, %index
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, read>, %coords:vec2<i32>, %index:u32):vec4<f32> {
  $B1: {
    %5:vec2<u32> = convert %coords
    %6:vec4<f32> = %t.read %5, %index
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_2darray_I32Index) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2dArray, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* index = b.FunctionParam("index", ty.i32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords, index});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords, index);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, read>, %coords:vec2<i32>, %index:i32):vec4<f32> {
  $B1: {
    %5:vec4<f32> = textureLoad %t, %coords, %index
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, read>, %coords:vec2<i32>, %index:i32):vec4<f32> {
  $B1: {
    %5:vec2<u32> = convert %coords
    %6:vec4<f32> = %t.read %5, %index
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_2darray_WithLevel) {
    auto* texture_ty =
        ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32());
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* index = b.FunctionParam("index", ty.i32());
    auto* level = b.FunctionParam("level", ty.i32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords, index, level});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords, index, level);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d_array<f32>, %coords:vec2<i32>, %index:i32, %level:i32):vec4<f32> {
  $B1: {
    %6:vec4<f32> = textureLoad %t, %coords, %index, %level
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d_array<f32>, %coords:vec2<i32>, %index:i32, %level:i32):vec4<f32> {
  $B1: {
    %6:vec2<u32> = convert %coords
    %7:vec4<f32> = %t.read %6, %index, %level
    ret %7
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureLoad_3d) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k3d, format, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec3<i32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_3d<rgba8unorm, read>, %coords:vec3<i32>):vec4<f32> {
  $B1: {
    %4:vec4<f32> = textureLoad %t, %coords
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_3d<rgba8unorm, read>, %coords:vec3<i32>):vec4<f32> {
  $B1: {
    %4:vec3<u32> = convert %coords
    %5:vec4<f32> = %t.read %4
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureNumLayers) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32()));
    auto* func = b.Function("foo", ty.u32());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<u32>(core::BuiltinFn::kTextureNumLayers, t);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d_array<f32>):u32 {
  $B1: {
    %3:u32 = textureNumLayers %t
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d_array<f32>):u32 {
  $B1: {
    %3:u32 = %t.get_array_size
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureNumLevels) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* func = b.Function("foo", ty.u32());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<u32>(core::BuiltinFn::kTextureNumLevels, t);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>):u32 {
  $B1: {
    %3:u32 = textureNumLevels %t
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>):u32 {
  $B1: {
    %3:u32 = %t.get_num_mip_levels
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureNumSamples) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::MultisampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* func = b.Function("foo", ty.u32());
    func->SetParams({t});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<u32>(core::BuiltinFn::kTextureNumSamples, t);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_multisampled_2d<f32>):u32 {
  $B1: {
    %3:u32 = textureNumSamples %t
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_multisampled_2d<f32>):u32 {
  $B1: {
    %3:u32 = %t.get_num_samples
    ret %3
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSample) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureSample, t, s, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = textureSample %t, %s, %coords
    ret %5
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>):vec4<f32> {
  $B1: {
    %5:vec4<f32> = %t.sample %s, %coords
    ret %5
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleBias) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* bias = b.FunctionParam("bias", ty.f32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, bias});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleBias, t, s, coords, bias);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %bias:f32):vec4<f32> {
  $B1: {
    %6:vec4<f32> = textureSampleBias %t, %s, %coords, %bias
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %bias:f32):vec4<f32> {
  $B1: {
    %6:msl.bias = construct %bias
    %7:vec4<f32> = %t.sample %s, %coords, %6
    ret %7
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleBias_Array) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* index = b.FunctionParam("index", ty.u32());
    auto* bias = b.FunctionParam("bias", ty.f32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, index, bias});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleBias, t, s, coords, index, bias);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d_array<f32>, %s:sampler, %coords:vec2<f32>, %index:u32, %bias:f32):vec4<f32> {
  $B1: {
    %7:vec4<f32> = textureSampleBias %t, %s, %coords, %index, %bias
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d_array<f32>, %s:sampler, %coords:vec2<f32>, %index:u32, %bias:f32):vec4<f32> {
  $B1: {
    %7:msl.bias = construct %bias
    %8:vec4<f32> = %t.sample %s, %coords, %index, %7
    ret %8
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleCompare) {
    auto* t =
        b.FunctionParam("t", ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d));
    auto* s = b.FunctionParam("s", ty.comparison_sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* depth = b.FunctionParam("depth", ty.f32());
    auto* func = b.Function("foo", ty.f32());
    func->SetParams({t, s, coords, depth});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<f32>(core::BuiltinFn::kTextureSampleCompare, t, s, coords, depth);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_depth_2d, %s:sampler_comparison, %coords:vec2<f32>, %depth:f32):f32 {
  $B1: {
    %6:f32 = textureSampleCompare %t, %s, %coords, %depth
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_depth_2d, %s:sampler_comparison, %coords:vec2<f32>, %depth:f32):f32 {
  $B1: {
    %6:f32 = %t.sample_compare %s, %coords, %depth
    ret %6
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleCompareLevel) {
    auto* t =
        b.FunctionParam("t", ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d));
    auto* s = b.FunctionParam("s", ty.comparison_sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* depth = b.FunctionParam("depth", ty.f32());
    auto* func = b.Function("foo", ty.f32());
    func->SetParams({t, s, coords, depth});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<f32>(core::BuiltinFn::kTextureSampleCompareLevel, t, s, coords, depth);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_depth_2d, %s:sampler_comparison, %coords:vec2<f32>, %depth:f32):f32 {
  $B1: {
    %6:f32 = textureSampleCompareLevel %t, %s, %coords, %depth
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_depth_2d, %s:sampler_comparison, %coords:vec2<f32>, %depth:f32):f32 {
  $B1: {
    %6:msl.level = construct 0u
    %7:f32 = %t.sample_compare %s, %coords, %depth, %6
    ret %7
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleCompareLevel_WithOffset) {
    auto* t =
        b.FunctionParam("t", ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d));
    auto* s = b.FunctionParam("s", ty.comparison_sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* depth = b.FunctionParam("depth", ty.f32());
    auto* offset = b.FunctionParam("offset", ty.vec2<i32>());
    auto* func = b.Function("foo", ty.f32());
    func->SetParams({t, s, coords, depth, offset});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<f32>(core::BuiltinFn::kTextureSampleCompareLevel, t, s, coords, depth, offset);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_depth_2d, %s:sampler_comparison, %coords:vec2<f32>, %depth:f32, %offset:vec2<i32>):f32 {
  $B1: {
    %7:f32 = textureSampleCompareLevel %t, %s, %coords, %depth, %offset
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_depth_2d, %s:sampler_comparison, %coords:vec2<f32>, %depth:f32, %offset:vec2<i32>):f32 {
  $B1: {
    %7:msl.level = construct 0u
    %8:f32 = %t.sample_compare %s, %coords, %depth, %7, %offset
    ret %8
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleGrad_2d) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* ddx = b.FunctionParam("ddx", ty.vec2<f32>());
    auto* ddy = b.FunctionParam("ddy", ty.vec2<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, ddx, ddy});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleGrad, t, s, coords, ddx, ddy);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %ddx:vec2<f32>, %ddy:vec2<f32>):vec4<f32> {
  $B1: {
    %7:vec4<f32> = textureSampleGrad %t, %s, %coords, %ddx, %ddy
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %ddx:vec2<f32>, %ddy:vec2<f32>):vec4<f32> {
  $B1: {
    %7:msl.gradient2d = construct %ddx, %ddy
    %8:vec4<f32> = %t.sample %s, %coords, %7
    ret %8
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleGrad_2dArray) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* index = b.FunctionParam("index", ty.i32());
    auto* ddx = b.FunctionParam("ddx", ty.vec2<f32>());
    auto* ddy = b.FunctionParam("ddy", ty.vec2<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, index, ddx, ddy});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleGrad, t, s, coords, index, ddx, ddy);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d_array<f32>, %s:sampler, %coords:vec2<f32>, %index:i32, %ddx:vec2<f32>, %ddy:vec2<f32>):vec4<f32> {
  $B1: {
    %8:vec4<f32> = textureSampleGrad %t, %s, %coords, %index, %ddx, %ddy
    ret %8
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d_array<f32>, %s:sampler, %coords:vec2<f32>, %index:i32, %ddx:vec2<f32>, %ddy:vec2<f32>):vec4<f32> {
  $B1: {
    %8:msl.gradient2d = construct %ddx, %ddy
    %9:vec4<f32> = %t.sample %s, %coords, %index, %8
    ret %9
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleGrad_3d) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k3d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec3<f32>());
    auto* ddx = b.FunctionParam("ddx", ty.vec3<f32>());
    auto* ddy = b.FunctionParam("ddy", ty.vec3<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, ddx, ddy});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleGrad, t, s, coords, ddx, ddy);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_3d<f32>, %s:sampler, %coords:vec3<f32>, %ddx:vec3<f32>, %ddy:vec3<f32>):vec4<f32> {
  $B1: {
    %7:vec4<f32> = textureSampleGrad %t, %s, %coords, %ddx, %ddy
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_3d<f32>, %s:sampler, %coords:vec3<f32>, %ddx:vec3<f32>, %ddy:vec3<f32>):vec4<f32> {
  $B1: {
    %7:msl.gradient3d = construct %ddx, %ddy
    %8:vec4<f32> = %t.sample %s, %coords, %7
    ret %8
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleGrad_Cube) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::kCube, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec3<f32>());
    auto* ddx = b.FunctionParam("ddx", ty.vec3<f32>());
    auto* ddy = b.FunctionParam("ddy", ty.vec3<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, ddx, ddy});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleGrad, t, s, coords, ddx, ddy);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_cube<f32>, %s:sampler, %coords:vec3<f32>, %ddx:vec3<f32>, %ddy:vec3<f32>):vec4<f32> {
  $B1: {
    %7:vec4<f32> = textureSampleGrad %t, %s, %coords, %ddx, %ddy
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_cube<f32>, %s:sampler, %coords:vec3<f32>, %ddx:vec3<f32>, %ddy:vec3<f32>):vec4<f32> {
  $B1: {
    %7:msl.gradientcube = construct %ddx, %ddy
    %8:vec4<f32> = %t.sample %s, %coords, %7
    ret %8
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleGrad_WithOffset) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* ddx = b.FunctionParam("ddx", ty.vec2<f32>());
    auto* ddy = b.FunctionParam("ddy", ty.vec2<f32>());
    auto* offset = b.FunctionParam("offset", ty.vec2<i32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, ddx, ddy, offset});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleGrad, t, s, coords, ddx, ddy, offset);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %ddx:vec2<f32>, %ddy:vec2<f32>, %offset:vec2<i32>):vec4<f32> {
  $B1: {
    %8:vec4<f32> = textureSampleGrad %t, %s, %coords, %ddx, %ddy, %offset
    ret %8
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %ddx:vec2<f32>, %ddy:vec2<f32>, %offset:vec2<i32>):vec4<f32> {
  $B1: {
    %8:msl.gradient2d = construct %ddx, %ddy
    %9:vec4<f32> = %t.sample %s, %coords, %8, %offset
    ret %9
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleLevel) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* level = b.FunctionParam("level", ty.f32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, level});
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleLevel, t, s, coords, level);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %level:f32):vec4<f32> {
  $B1: {
    %6:vec4<f32> = textureSampleLevel %t, %s, %coords, %level
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d<f32>, %s:sampler, %coords:vec2<f32>, %level:f32):vec4<f32> {
  $B1: {
    %6:msl.level = construct %level
    %7:vec4<f32> = %t.sample %s, %coords, %6
    ret %7
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureSampleLevel_Array) {
    auto* t = b.FunctionParam(
        "t", ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32()));
    auto* s = b.FunctionParam("s", ty.sampler());
    auto* coords = b.FunctionParam("coords", ty.vec2<f32>());
    auto* index = b.FunctionParam("index", ty.u32());
    auto* level = b.FunctionParam("level", ty.f32());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, s, coords, index, level});
    b.Append(func->Block(), [&] {
        auto* result =
            b.Call<vec4<f32>>(core::BuiltinFn::kTextureSampleLevel, t, s, coords, index, level);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_2d_array<f32>, %s:sampler, %coords:vec2<f32>, %index:u32, %level:f32):vec4<f32> {
  $B1: {
    %7:vec4<f32> = textureSampleLevel %t, %s, %coords, %index, %level
    ret %7
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_2d_array<f32>, %s:sampler, %coords:vec2<f32>, %index:u32, %level:f32):vec4<f32> {
  $B1: {
    %7:msl.level = construct %level
    %8:vec4<f32> = %t.sample %s, %coords, %index, %7
    ret %8
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_1d_U32Coord) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k1d, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.u32());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, write>, %coords:u32, %value:vec4<f32>):void {
  $B1: {
    %5:void = textureStore %t, %coords, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, write>, %coords:u32, %value:vec4<f32>):void {
  $B1: {
    %5:void = %t.write %value, %coords
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_1d_I32Coord) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k1d, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.i32());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, write>, %coords:i32, %value:vec4<f32>):void {
  $B1: {
    %5:void = textureStore %t, %coords, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_1d<rgba8unorm, write>, %coords:i32, %value:vec4<f32>):void {
  $B1: {
    %5:u32 = convert %coords
    %6:void = %t.write %value, %5
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_2d_U32Coords) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<u32>());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, write>, %coords:vec2<u32>, %value:vec4<f32>):void {
  $B1: {
    %5:void = textureStore %t, %coords, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, write>, %coords:vec2<u32>, %value:vec4<f32>):void {
  $B1: {
    %5:void = %t.write %value, %coords
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_2d_I32Coords) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, write>, %coords:vec2<i32>, %value:vec4<f32>):void {
  $B1: {
    %5:void = textureStore %t, %coords, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, write>, %coords:vec2<i32>, %value:vec4<f32>):void {
  $B1: {
    %5:vec2<u32> = convert %coords
    %6:void = %t.write %value, %5
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_2darray_U32Index) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2dArray, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* index = b.FunctionParam("index", ty.u32());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, index, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, index, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, write>, %coords:vec2<i32>, %index:u32, %value:vec4<f32>):void {
  $B1: {
    %6:void = textureStore %t, %coords, %index, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, write>, %coords:vec2<i32>, %index:u32, %value:vec4<f32>):void {
  $B1: {
    %6:vec2<u32> = convert %coords
    %7:void = %t.write %value, %6, %index
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_2darray_I32Index) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2dArray, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* index = b.FunctionParam("index", ty.i32());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, index, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, index, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, write>, %coords:vec2<i32>, %index:i32, %value:vec4<f32>):void {
  $B1: {
    %6:void = textureStore %t, %coords, %index, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d_array<rgba8unorm, write>, %coords:vec2<i32>, %index:i32, %value:vec4<f32>):void {
  $B1: {
    %6:vec2<u32> = convert %coords
    %7:void = %t.write %value, %6, %index
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureStore_3d) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k3d, format, core::Access::kWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec3<i32>());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.void_());
    func->SetParams({t, coords, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, value);
        b.Return(func);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_3d<rgba8unorm, write>, %coords:vec3<i32>, %value:vec4<f32>):void {
  $B1: {
    %5:void = textureStore %t, %coords, %value
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_3d<rgba8unorm, write>, %coords:vec3<i32>, %value:vec4<f32>):void {
  $B1: {
    %5:vec3<u32> = convert %coords
    %6:void = %t.write %value, %5
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

// Test that we insert a fence after the store to ensure that it is ordered before the load.
TEST_F(MslWriter_BuiltinPolyfillTest, TextureStoreToReadWriteBeforeLoad) {
    auto format = core::TexelFormat::kRgba8Unorm;
    auto* texture_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, format, core::Access::kReadWrite,
        core::type::StorageTexture::SubtypeFor(format, ty));
    auto* t = b.FunctionParam("t", texture_ty);
    auto* coords = b.FunctionParam("coords", ty.vec2<i32>());
    auto* value = b.FunctionParam("value", ty.vec4<f32>());
    auto* func = b.Function("foo", ty.vec4<f32>());
    func->SetParams({t, coords, value});
    b.Append(func->Block(), [&] {
        b.Call<void>(core::BuiltinFn::kTextureStore, t, coords, value);
        auto* result = b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, t, coords);
        b.Return(func, result);
    });

    auto* src = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, read_write>, %coords:vec2<i32>, %value:vec4<f32>):vec4<f32> {
  $B1: {
    %5:void = textureStore %t, %coords, %value
    %6:vec4<f32> = textureLoad %t, %coords
    ret %6
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = func(%t:texture_storage_2d<rgba8unorm, read_write>, %coords:vec2<i32>, %value:vec4<f32>):vec4<f32> {
  $B1: {
    %5:vec2<u32> = convert %coords
    %6:void = %t.write %value, %5
    %7:void = %t.fence
    %8:vec2<u32> = convert %coords
    %9:vec4<f32> = %t.read %8
    ret %9
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, WorkgroupBarrier) {
    auto* func = b.Function("foo", ty.void_());
    func->SetStage(core::ir::Function::PipelineStage::kCompute);
    func->SetWorkgroupSize(1, 1, 1);
    b.Append(func->Block(), [&] {
        b.Call(ty.void_(), core::BuiltinFn::kWorkgroupBarrier);
        b.Return(func);
    });

    auto* src = R"(
%foo = @compute @workgroup_size(1, 1, 1) func():void {
  $B1: {
    %2:void = workgroupBarrier
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = @compute @workgroup_size(1, 1, 1) func():void {
  $B1: {
    %2:void = msl.threadgroup_barrier 4u
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, StorageBarrier) {
    auto* func = b.Function("foo", ty.void_());
    func->SetStage(core::ir::Function::PipelineStage::kCompute);
    func->SetWorkgroupSize(1, 1, 1);
    b.Append(func->Block(), [&] {
        b.Call(ty.void_(), core::BuiltinFn::kStorageBarrier);
        b.Return(func);
    });

    auto* src = R"(
%foo = @compute @workgroup_size(1, 1, 1) func():void {
  $B1: {
    %2:void = storageBarrier
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = @compute @workgroup_size(1, 1, 1) func():void {
  $B1: {
    %2:void = msl.threadgroup_barrier 1u
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

TEST_F(MslWriter_BuiltinPolyfillTest, TextureBarrier) {
    auto* func = b.Function("foo", ty.void_());
    func->SetStage(core::ir::Function::PipelineStage::kCompute);
    func->SetWorkgroupSize(1, 1, 1);
    b.Append(func->Block(), [&] {
        b.Call(ty.void_(), core::BuiltinFn::kTextureBarrier);
        b.Return(func);
    });

    auto* src = R"(
%foo = @compute @workgroup_size(1, 1, 1) func():void {
  $B1: {
    %2:void = textureBarrier
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%foo = @compute @workgroup_size(1, 1, 1) func():void {
  $B1: {
    %2:void = msl.threadgroup_barrier 2u
    ret
  }
}
)";

    Run(BuiltinPolyfill);

    EXPECT_EQ(expect, str());
}

}  // namespace
}  // namespace tint::msl::writer::raise
