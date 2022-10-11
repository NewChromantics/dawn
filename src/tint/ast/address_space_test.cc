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
//   src/tint/ast/address_space_test.cc.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "src/tint/ast/address_space.h"

#include <string>

#include "src/tint/ast/test_helper.h"
#include "src/tint/utils/string.h"

namespace tint::ast {
namespace {

namespace parse_print_tests {

struct Case {
    const char* string;
    AddressSpace value;
};

inline std::ostream& operator<<(std::ostream& out, Case c) {
    return out << "'" << std::string(c.string) << "'";
}

static constexpr Case kValidCases[] = {
    {"function", AddressSpace::kFunction},
    {"private", AddressSpace::kPrivate},
    {"push_constant", AddressSpace::kPushConstant},
    {"storage", AddressSpace::kStorage},
    {"uniform", AddressSpace::kUniform},
    {"workgroup", AddressSpace::kWorkgroup},
};

static constexpr Case kInvalidCases[] = {
    {"fccnctin", AddressSpace::kInvalid},        {"ucti3", AddressSpace::kInvalid},
    {"functVon", AddressSpace::kInvalid},        {"priv1te", AddressSpace::kInvalid},
    {"pqiJate", AddressSpace::kInvalid},         {"privat7ll", AddressSpace::kInvalid},
    {"pqqsh_pponstHnt", AddressSpace::kInvalid}, {"pus_cnstat", AddressSpace::kInvalid},
    {"bus_Gonstant", AddressSpace::kInvalid},    {"storiive", AddressSpace::kInvalid},
    {"8WWorage", AddressSpace::kInvalid},        {"sxxrage", AddressSpace::kInvalid},
    {"uXforgg", AddressSpace::kInvalid},         {"nfoXm", AddressSpace::kInvalid},
    {"unif3rm", AddressSpace::kInvalid},         {"workgroEp", AddressSpace::kInvalid},
    {"woTTPkroup", AddressSpace::kInvalid},      {"ddorkroxxp", AddressSpace::kInvalid},
};

using AddressSpaceParseTest = testing::TestWithParam<Case>;

TEST_P(AddressSpaceParseTest, Parse) {
    const char* string = GetParam().string;
    AddressSpace expect = GetParam().value;
    EXPECT_EQ(expect, ParseAddressSpace(string));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, AddressSpaceParseTest, testing::ValuesIn(kValidCases));
INSTANTIATE_TEST_SUITE_P(InvalidCases, AddressSpaceParseTest, testing::ValuesIn(kInvalidCases));

using AddressSpacePrintTest = testing::TestWithParam<Case>;

TEST_P(AddressSpacePrintTest, Print) {
    AddressSpace value = GetParam().value;
    const char* expect = GetParam().string;
    EXPECT_EQ(expect, utils::ToString(value));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, AddressSpacePrintTest, testing::ValuesIn(kValidCases));

}  // namespace parse_print_tests

}  // namespace
}  // namespace tint::ast
