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

////////////////////////////////////////////////////////////////////////////////
// File generated by tools/src/cmd/gen
// using the template:
//   src/tint/ast/builtin_value.h.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_AST_BUILTIN_VALUE_H_
#define SRC_TINT_AST_BUILTIN_VALUE_H_

#include <ostream>

namespace tint::ast {

/// Storage class of a given pointer.
enum class BuiltinValue {
    kInvalid,
    kFragDepth,
    kFrontFacing,
    kGlobalInvocationId,
    kInstanceIndex,
    kLocalInvocationId,
    kLocalInvocationIndex,
    kNumWorkgroups,
    kPointSize,  // Tint-internal enum entry - not parsed
    kPosition,
    kSampleIndex,
    kSampleMask,
    kVertexIndex,
    kWorkgroupId,
};

/// @param out the std::ostream to write to
/// @param value the BuiltinValue
/// @returns `out` so calls can be chained
std::ostream& operator<<(std::ostream& out, BuiltinValue value);

/// ParseBuiltinValue parses a BuiltinValue from a string.
/// @param str the string to parse
/// @returns the parsed enum, or BuiltinValue::kInvalid if the string could not be parsed.
BuiltinValue ParseBuiltinValue(std::string_view str);

}  // namespace tint::ast

#endif  // SRC_TINT_AST_BUILTIN_VALUE_H_
