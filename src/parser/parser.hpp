#pragma once

#include "dom.hpp"
#include "tokenizer.hpp"

#include <memory>
#include <stack>

std::unique_ptr<Dom> parse_tokens(std::vector<Token> tokens);
void parse_content(Dom& dom, std::vector<Token> token);
void parse_tag(Dom& dom, std::vector<Token> token);
void parse_attributes(Dom& dom, std::vector<Token> token);
