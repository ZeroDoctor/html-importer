#include "tokenizer.hpp"
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
		
		while(m_col < m_current_line_length) {
			Token& prev = getPreviousToken(tokens);
		
			std::vector<Token> temp = process_tag(prev, line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col < m_current_line_length) break;
			
			temp = process_tag_name(prev, line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			
			prev = tokens[tokens.size()-1];
			temp = process_comment(prev, line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col < m_current_line_length) break;
			
			prev = tokens[tokens.size()-1];
			temp = process_attribute(prev, line);
			tokens.insert(tokens.end(), temp.begin(), temp.end());
			if(m_col < m_current_line_length) break;
			
			process_content(line);
		}
		
		m_row++;	
	}
	
	return tokens;
}


std::vector<Token> Lexer::process_tag(Token& previous_token, std::string line) 
{
	std::vector<Token> tokens;
	
	if(line[m_col]!='<'&& line[m_col]!='>') return tokens;
	else if(m_col+3 < m_current_line_length && line.substr(m_col+1, 3) == "!--") return tokens;
	else if(previous_token.type == TOKEN_COMMENT_LESS) return tokens;

	Token token = {};

	token.start_row = m_row;
	token.start_col = m_col;
	token.end_row = m_row;
	token.end_col = m_col;
	token.text += line[m_col];
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
	else if(previous_token.type == TOKEN_COMMENT_LESS) return tokens;
	
	m_col = first_non_space(line, m_col);
	if(m_col == std::string::npos)
	{
		m_col = m_current_line_length;
		return tokens;
	} 
	
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
		token.end_col = m_col;
		token.text = name;
		tokens.push_back(token);
	}
	
	m_col++;
	return tokens;
}

std::vector<Token> Lexer::process_comment(Token& previous_token, std::string line) 
{
	std::vector<Token> tokens;
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
	if(previous_token.type == TOKEN_COMMENT_LESS) return tokens;
	if(previous_token.type != TOKEN_TAG_LESS) return tokens;
	
	
	return tokens;
}

std::vector<Token> Lexer::process_content(std::string line) 
{
	std::vector<Token> tokens;
	// check for end comment bracket
	return tokens;
}

