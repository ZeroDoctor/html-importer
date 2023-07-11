#pragma once

#include "dom.hpp"
#include "tokenizer.hpp"
#include "spdlog/spdlog.h"

#include <memory>
#include <stack>

std::unique_ptr<Dom> parse_tokens(std::vector<Token> tokens);
size_t parse_content(Dom& dom, std::vector<Token> tokens, size_t index);
size_t parse_tag(Dom* dom, std::vector<Token> tokens, size_t index);
size_t parse_attributes(Dom& dom, std::vector<Token> tokens, size_t index);
