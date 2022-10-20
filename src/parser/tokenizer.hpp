#pragma once

#include <vector>
#include <string>

inline const char* EMPTY_TAG_NAME = "!--empty--";
inline const char* COMMENT_TAG_NAME = "!--comment--";

enum TokenType {
	TOKEN_NOTHING,
	TOKEN_START_TAG,
	TOKEN_END_TAG,
	TOKEN_ATTRIBUTE,
	TOKEN_CONTENT,
	TOKEN_START_COMMENT,
	TOKEN_END_COMMENT,
};

struct Token {
	TokenType type = TOKEN_NOTHING;
	std::string token = "";
	
	size_t start_row = -1;
	size_t start_col = -1;
	
	size_t end_row = -1;
	size_t end_col = -1;

	bool is_complete() {
		return (type == TOKEN_NOTHING) || end_row != -1 && end_col != -1;
	}
};

class Lexer {
public:
	Lexer();
	std::vector<Token> tokenizer(std::vector<std::string> lines);

private:
	~Lexer();

	void process_tag(Token& current_token, std::string line);
	void process_attribute(Token& current_token, std::string line);
	void process_content(Token& current_token, std::string line);

	inline size_t first_non_space(std::string line, size_t pos) {
		size_t result = std::string::npos;

		size_t col = pos;
		while(col < line.length()) {
			if(!isspace(line[col]))
				return col;
			
			col++;
		}

		return result;
	}

	int m_row = 0;
	int m_col = 0;
	int m_current_line_length = 0;
};
