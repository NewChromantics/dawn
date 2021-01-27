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

#include "src/reader/spirv/parser.h"

#include <utility>

#include "src/reader/spirv/parser_impl.h"

namespace tint {
namespace reader {
namespace spirv {

Parser::Parser(const std::vector<uint32_t>& spv_binary)
    : Reader(), impl_(std::make_unique<ParserImpl>(spv_binary)) {}

Parser::~Parser() = default;

bool Parser::Parse() {
  const auto result = impl_->Parse();
  auto err_msg = impl_->error();
  if (!err_msg.empty()) {
    // TODO(bclayton): Migrate spirv::ParserImpl to using diagnostics.
    diag::List diagnostics;
    diagnostics.add_error(err_msg);
    set_diagnostics(std::move(diagnostics));
  }
  return result;
}

Program Parser::program() {
  return impl_->program();
}

Program Parse(const std::vector<uint32_t>& input) {
  ParserImpl parser(input);
  bool parsed = parser.Parse();
  ProgramBuilder builder = std::move(parser.builder());
  if (!parsed) {
    // TODO(bclayton): Migrate spirv::ParserImpl to using diagnostics.
    builder.Diagnostics().add_error(parser.error());
  }
  return Program(std::move(builder));
}

}  // namespace spirv
}  // namespace reader
}  // namespace tint
