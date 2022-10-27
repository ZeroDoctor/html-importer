#pragma once

#include <iostream>
#include <cctype>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "spdlog/spdlog.h"

// TODO: move this later
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
	if(size_s <= 0) throw std::runtime_error("error during formatting.");
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ... );
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

static std::string convert_space_str(std::string str) 
{
	std::string result;
	for(auto c : str) {
		if(std::isspace(c)) 
		{
			result += ' ';
			continue;
		}
		result += c;
	}

	return result;
}

static char convert_space(char c) 
{
	if(std::isspace(c)) return ' ';
	return c;
}


inline const char* EMPTY_TAG_NAME = "!--empty--";

enum TokenType {
	TOKEN_CONTENT=0,
	TOKEN_TAG_LESS,
	TOKEN_TAG_LESS_DASH,
	TOKEN_TAG_GREATER,
	TOKEN_TAG_NAME,
	TOKEN_COMMENT_LESS,
	TOKEN_COMMENT_GREATER,
	TOKEN_ATTRIBUTE_NAME,
	TOKEN_ATTRIBUTE_EQUAL,
	TOKEN_ATTRIBUTE_VALUE,
	TOKEN_ATTRIBUTE_VALUE_START,
	TOKEN_ATTRIBUTE_VALUE_END,
};

const char SINGLE_QUOTE = '\'';
const char DOUBLE_QUOTE = '"';

struct Token {
	TokenType type = TOKEN_CONTENT;
	char attr_quote_type = ' ';
	std::string text;
	
	size_t start_row = -1;
	size_t start_col = -1;
	
	size_t end_row = -1;
	size_t end_col = -1;

	bool is_complete() {
		return (type == TOKEN_CONTENT) || end_row != -1 && end_col != -1;
	}

	std::string to_string() {
		return string_format(
			"[start_row=%d] [start_col=%d] [text=%s] [end_row=%d] [end_col=%d] [type=%d]", 
			start_row, start_col, text.c_str(), end_row, end_col, type);
	}
};

class Lexer {
public:
	Lexer();
	~Lexer();
	std::vector<Token> tokenizer(std::vector<std::string> lines);

private:

	std::vector<Token> process_tag(Token& previous_token, std::string line);
	std::vector<Token> process_comment(Token& previous_token, std::string line);
	std::vector<Token> process_tag_name(Token& previous_token, std::string line);
	std::vector<Token> process_attribute(Token& previous_token, std::string line);
	std::vector<Token> process_content(Token& previous_token, std::string line);

	inline size_t first_non_space(std::string line, size_t pos) 
	{
		size_t result = std::string::npos;
		
		size_t col = pos;
		while(col < line.length()) {
			if(!std::isspace(line[col]))
				return col;
			
			col++;
		}
		
		return result;
	}

	inline size_t first_space(std::string line, size_t pos)
	{
		size_t result = std::string::npos;
		size_t col = pos;
		while(col < line.length()) {
			if (std::isspace(line[col]))
				return col;
				
			col++;
		}
		
		return result;
	}

	inline size_t first_non_attr_name(std::string line, size_t pos) 
	{
		size_t result = std::string::npos;

		size_t col = pos;
		while(col < line.length()) {
			if(
				std::isspace(line[col]) || 
				line[col] == '=' || 
				line[col] == '>' || 
				line[col] == '<' || 
				line[col] == '"' || 
				line[col] == '\''
			) return col;
			
			col++;
		}

		return result;
	}

	int m_row = 0;
	int m_col = 0;
	int m_current_line_length = 0;
};
