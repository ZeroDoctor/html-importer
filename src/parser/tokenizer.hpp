#pragma once

#include <cctype>
#include <vector>
#include <string>

inline const char* EMPTY_TAG_NAME = "!--empty--";

enum TokenType {
	TOKEN_CONTENT,
	TOKEN_TAG_LESS,
	TOKEN_TAG_GREATER,
	TOKEN_COMMENT_LESS,
	TOKEN_COMMENT_GREATER,
	TOKEN_TAG_NAME,
	TOKEN_ATTRIBUTE,
};

struct Token {
	TokenType type = TOKEN_CONTENT;
	std::string text = "";
	
	size_t start_row = -1;
	size_t start_col = -1;
	
	size_t end_row = -1;
	size_t end_col = -1;

	bool is_complete() {
		return (type == TOKEN_CONTENT) || end_row != -1 && end_col != -1;
	}
};

class Lexer {
public:
	Lexer();
	std::vector<Token> tokenizer(std::vector<std::string> lines);

private:
	~Lexer();

	std::vector<Token> process_tag(Token& previous_token, std::string line);
	std::vector<Token> process_comment(Token& previous_token, std::string line);
	std::vector<Token> process_tag_name(Token& previous_token, std::string line);
	std::vector<Token> process_attribute(Token& previous_token, std::string line);
	std::vector<Token> process_content(std::string line);

	inline size_t first_non_space(std::string line, size_t pos) {
		size_t result = std::string::npos;
		
		size_t col = pos;
		while(col < line.length()) {
			if(!std::isspace(line[col]))
				return col;
			
			col++;
		}
		
		return result;
	}

	inline size_t first_valid_tag_name(std::string line, size_t pos) {
		size_t result = std::string::npos;

		size_t col = pos;
		while(col < line.length()) {
			if(!std::isspace(line[col]) && std::isalnum(line[col]))
				return col;
			else if(!std::isspace(line[col]) && !std::isalnum(line[col]))
				return result;
			
			col++;
		}

		return result;
	}

	int m_row = 0;
	int m_col = 0;
	int m_current_line_length = 0;
};
