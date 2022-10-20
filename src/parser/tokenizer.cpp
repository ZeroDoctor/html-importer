#include "tokenizer.hpp"
#include <cctype>
#include <stack>

Lexer::Lexer() { }
Lexer::~Lexer() { }

std::vector<Token> Lexer::tokenizer(std::vector<std::string> lines) 
{
	std::vector<Token> tokens = {};
	std::stack<Token> current_token_stack;
	int window = 0; 

	for(auto line : lines) {
		m_current_line_length = line.length();
		
		Token current_token = {};
		if(current_token_stack.size() > 0) 
		{
			current_token = current_token_stack.top();
			current_token_stack.pop();
		}
		
		process_tag(current_token, line);
		if(!current_token.is_complete()) 
			current_token_stack.push(current_token);
		else if(current_token.is_complete() && current_token.type != TOKEN_NOTHING) 
			tokens.push_back(current_token);
		if(m_col >= m_current_line_length) continue;
		
		current_token = {};
		process_attribute(current_token, line);
		if(!current_token.is_complete()) 
			current_token_stack.push(current_token);
		else if(current_token.is_complete() && current_token.type != TOKEN_NOTHING) 
			tokens.push_back(current_token);
		if(m_col >= m_current_line_length) continue;
		
		current_token = {};
		process_content(current_token, line);
		if(!current_token.is_complete()) 
			current_token_stack.push(current_token);
		else if(current_token.is_complete() && current_token.type != TOKEN_NOTHING) 
			tokens.push_back(current_token);
		if(m_col >= m_current_line_length) continue;
		
		m_row++;	
	}
	
	return tokens;
}


void Lexer::process_tag(Token& current_token, std::string line) 
{
	if(
		line[m_col] != '<' && 
		line[m_col] != '>' &&
		current_token.type != TOKEN_NOTHING && 
		current_token.type != TOKEN_START_TAG && 
		current_token.type != TOKEN_END_TAG
	) return;

	if(line[m_col] == '>')
	{
		current_token.end_row = m_row;
		current_token.end_col = m_col;

		if(line.substr(m_col-2, 3) == "-->") {
			current_token.type = TOKEN_END_COMMENT;
			current_token.start_row = m_row;
			current_token.start_col = m_col-2;
			current_token.token = COMMENT_TAG_NAME;
		}
		
		return;
	}

	if(line[m_col] == '<') 
	{
		current_token.start_row = m_row;
		current_token.start_col = m_col;
	}
	
	if(current_token.type == TOKEN_NOTHING) 
	{
		current_token.type = TOKEN_START_TAG;
		if (m_col+1 < m_current_line_length && line[m_col+1] == '/') 
		{
			current_token.type = TOKEN_END_TAG;
			m_col++;
		}
		
		m_col++;
		if(m_col >= m_current_line_length) return;
	}

	if(current_token.token != "") return;
	
	size_t bracket_index = line.find('>', m_col);
	size_t non_space_index = first_non_space(line, m_col);
	if(bracket_index == std::string::npos && non_space_index == std::string::npos) return;
	
	if(bracket_index != std::string::npos && 
		non_space_index != std::string::npos && 
		bracket_index < non_space_index
	)
	{
		current_token.token = EMPTY_TAG_NAME;
		return;
	}

	if(non_space_index != std::string::npos)
	{
		std::string token = "";
		while(m_col < m_current_line_length) {
			if (isspace(line[m_col])) 
			{
				current_token.token = token;
				break;
			}
			
			token += line[m_col];
			m_col++;
		}
		
		size_t offset = m_col - token.length();
		if (token.substr(offset, 3) == "!--")
		{
			current_token.type = TOKEN_START_COMMENT;
			current_token.token = COMMENT_TAG_NAME;
			return;
		}
	}

	if (bracket_index != std::string::npos)
	{
		current_token.end_row = m_row;
		current_token.end_col = bracket_index;
	}
}

void Lexer::process_attribute(Token& current_token, std::string line) 
{
	current_token.type = TOKEN_ATTRIBUTE;

	current_token.start_row = m_row;
	current_token.start_col = m_col;

	m_col++;
	if(m_col >= m_current_line_length) return;
}

void Lexer::process_content(Token& current_token, std::string line) 
{
	current_token.type = TOKEN_CONTENT;

	current_token.start_row = m_row;
	current_token.start_col = m_col;

	m_col++;
	if(m_col >= m_current_line_length) return;
}

