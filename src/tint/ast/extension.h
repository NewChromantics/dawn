// Copyright 2022 The Tint Authors.
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
//   src/tint/ast/extension.h.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_AST_EXTENSION_H_
#define SRC_TINT_AST_EXTENSION_H_

#include <ostream>

#include "src/tint/utils/unique_vector.h"

namespace tint::ast {

/// An enumerator of WGSL extensions
/// @see src/tint/intrinsics.def for extension descriptions
enum class Extension {
    kInvalid,
    kChromiumDisableUniformityAnalysis,
    kChromiumExperimentalDp4A,
    kChromiumExperimentalPushConstant,
    kF16,
};

/// @param out the std::ostream to write to
/// @param value the Extension
/// @returns `out` so calls can be chained
std::ostream& operator<<(std::ostream& out, Extension value);

/// ParseExtension parses a Extension from a string.
/// @param str the string to parse
/// @returns the parsed enum, or Extension::kInvalid if the string could not be parsed.
Extension ParseExtension(std::string_view str);

// A unique vector of extensions
using Extensions = utils::UniqueVector<Extension, 4>;

}  // namespace tint::ast

#endif  // SRC_TINT_AST_EXTENSION_H_
