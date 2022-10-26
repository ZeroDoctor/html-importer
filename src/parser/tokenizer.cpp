#include "tokenizer.hpp"
#include "spdlog/spdlog.h"
#include <cctype>
#include <stack>

Token empty = {};

Token& getPreviousToken(std::vector<Token>& tokens) {
	if(tokens.size() > 0) {
		return tokens[tokens.size()-1];
	}
	
	return empty;
}

Lexer::Lexer() { }
Lexer::~Lexer() { }

std::vector<Token> Lexer::tokenizer(std::vector<std::string> lines) 
{
	std::vector<Token> tokens = {};

	for(auto line : lines) {
		m_current_line_length = line.length();
		
		spdlog::debug("line={}", line.c_str());
		while(m_col < m_current_line_length) {
			std::vector<Token> temp = process_tag(getPreviousToken(tokens), line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col >= m_current_line_length) break;
			if(temp.size() > 0) continue;
			
			temp = process_tag_name(getPreviousToken(tokens), line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col >= m_current_line_length) break;
			if(temp.size() > 0) continue;
			
			temp = process_comment(getPreviousToken(tokens), line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col >= m_current_line_length) break;
			if(temp.size() > 0) continue;
			
			temp = process_attribute(getPreviousToken(tokens), line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col >= m_current_line_length) break;
			if(temp.size() > 0) continue;
			
			temp = process_content(getPreviousToken(tokens), line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
		}
		
		m_col = 0;
		m_row++;	
	}
	
	return tokens;
}


std::vector<Token> Lexer::process_tag(Token& previous_token, std::string line) 
{
	std::vector<Token> tokens;
	
	if(line[m_col]!='<'&& line[m_col]!='>') return tokens;
	// else if(m_col+3 < m_current_line_length && line.substr(m_col+1, 3) == "!--") return tokens;
	else if(previous_token.type == TOKEN_COMMENT_LESS) return tokens;
	if(m_row == 1) spdlog::debug("in tag");

	if(previous_token.type == TOKEN_CONTENT) 
	{
		previous_token.end_col = m_col;
		previous_token.end_row = m_row;
	}

	Token token = {};

	token.start_row = m_row;
	token.start_col = m_col;
	token.end_row = m_row;
	token.end_col = m_col;
	token.text = "";
	token.text.push_back(convert_space(line[m_col]));
	if (token.text == "<") 
	{
		token.type = TOKEN_TAG_LESS;
	} else if (token.text == ">") {
		token.type = TOKEN_TAG_GREATER;
	}
	tokens.push_back(token);
	
	m_col++;
	return tokens;
}

std::vector<Token> Lexer::process_tag_name(Token& previous_token, std::string line)  
{
	std::vector<Token> tokens;
	
	if(previous_token.type != TOKEN_TAG_LESS) return tokens;
	else if(m_col+2 < m_current_line_length && line.substr(m_col, 3) == "!--") return tokens;
	
	std::string name = "";
	while(m_col < m_current_line_length && std::isalnum(line[m_col])) {
		name += line[m_col];
		m_col++;
	}

	if (name != "") 
	{
		Token token = {};
		token.start_row = m_row;
		token.start_col = m_col - name.length();
		token.end_row = m_row;
		token.end_col = m_col-1;
		token.text.append(convert_space_str(name));
		token.type = TOKEN_TAG_NAME;
		tokens.push_back(token);
	}
	
	return tokens;
}

std::vector<Token> Lexer::process_comment(Token& previous_token, std::string line) 
{
	std::vector<Token> tokens = {};
	if(previous_token.type != TOKEN_TAG_LESS) return tokens;
	else if(m_col+2 < m_current_line_length && line.substr(m_col, 3) != "!--") return tokens;
	
	previous_token.type = TOKEN_COMMENT_LESS;
	previous_token.end_col = m_col;
	
	m_col++;
	return tokens;
}

std::vector<Token> Lexer::process_attribute(Token& previous_token, std::string line) 
{
	std::vector<Token> tokens;
	if(previous_token.type != TOKEN_TAG_NAME) return tokens;
	
	if(previous_token.type == TOKEN_ATTRIBUTE && line[m_col] == '=') 
	{
		previous_token.end_row = m_row;
		previous_token.end_col = m_col - 1;
		
		Token token = {};

		m_col++;
		if (m_col < m_current_line_length) 
		{
			token.text.push_back(convert_space(line[m_col]));
		}

		token.type = TOKEN_ATTRIBUTE_VALUE;
		token.start_row = m_row;
		token.start_col = m_col;
		tokens.push_back(token);

		return tokens;
	}

	if(previous_token.type == TOKEN_ATTRIBUTE)
	{
		previous_token.text.push_back(convert_space(line[m_col]));
		
		return tokens;
	} else if (previous_token.type == TOKEN_ATTRIBUTE_VALUE) {
		while( (previous_token.start_col+1 != m_col || previous_token.start_row != m_row) && line[m_col] != '"') {
			previous_token.text.push_back(convert_space(line[m_col]));
			m_col++;
			if (m_col > m_current_line_length) {
				return tokens;
			}
		}
	}

	Token token = {};
	token.type = TOKEN_ATTRIBUTE;
	token.start_row = m_row;
	token.start_col = m_col;
	token.text.push_back(convert_space(line[m_col]));
	tokens.push_back(token);
	
	m_col++;
	return tokens;
}

std::vector<Token> Lexer::process_content(Token& previous_token, std::string line) 
{
	std::vector<Token> tokens;
	if(previous_token.type == TOKEN_CONTENT) 
	{
		previous_token.text.push_back(convert_space(line[m_col]));
		m_col++;
		return tokens;
	}

	Token token = {};
	token.type = TOKEN_CONTENT;
	token.start_row = m_row;
	token.start_col = m_col;
	token.text.push_back(convert_space(line[m_col]));
	tokens.push_back(token);
	
	m_col++;
	return tokens;
}

