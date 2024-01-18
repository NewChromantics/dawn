// Copyright 2019 The Dawn & Tint Authors
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

#ifndef SRC_DAWN_WIRE_WIREDESERIALIZEALLOCATOR_H_
#define SRC_DAWN_WIRE_WIREDESERIALIZEALLOCATOR_H_

#include <vector>

#include "dawn/wire/WireCmd_autogen.h"
#include "partition_alloc/pointers/raw_ptr.h"

namespace dawn::wire {
// A really really simple implementation of the DeserializeAllocator. It's main feature
// is that it has some inline storage so as to avoid allocations for the majority of
// commands.
class WireDeserializeAllocator : public DeserializeAllocator {
  public:
    WireDeserializeAllocator();
    virtual ~WireDeserializeAllocator();

    void* GetSpace(size_t size) override;

    void Reset();

  private:
    size_t mRemainingSize = 0;
    // TODO(https://crbug.com/dawn/2345): Investigate `DanglingUntriaged` in dawn/wire.
    raw_ptr<char, AllowPtrArithmetic | DanglingUntriaged> mCurrentBuffer = nullptr;
    char mStaticBuffer[2048];
    std::vector<char*> mAllocations;
};
}  // namespace dawn::wire

#endif  // SRC_DAWN_WIRE_WIREDESERIALIZEALLOCATOR_H_
