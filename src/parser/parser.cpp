#include "parser.hpp"

std::unique_ptr<Dom> parse_tokens(std::vector<Token> tokens)
{
	Tag rootTag = {};
	std::unique_ptr<Dom> root(new Dom(rootTag, NULL));

	// create stack
	std::stack<Dom*> dom_stack;
	dom_stack.push(root.get());

	for(Token token : tokens) {
		Dom* parent = dom_stack.top();
		dom_stack.pop();
			
		if(token.type == TOKEN_CONTENT) {
			
		}

		switch(token.type) {
			case TOKEN_CONTENT:
			break;
			case TOKEN_TAG_LESS:
			case TOKEN_TAG_LESS_DASH:
			case TOKEN_TAG_GREATER:
			case TOKEN_TAG_GREATER_DASH:
			case TOKEN_TAG_NAME:
			break;
			case TOKEN_ATTRIBUTE_NAME:
			case TOKEN_ATTRIBUTE_EQUAL:
			case TOKEN_ATTRIBUTE_VALUE:
			case TOKEN_ATTRIBUTE_VALUE_START:
			case TOKEN_ATTRIBUTE_VALUE_END:
			break;
		}

		// if end tag is found
		// grab top of stack and append children
	}

	return root;
}

void parse_content(Dom& dom, std::vector<Token> token)
{

}

void parse_tag(Dom& dom, std::vector<Token> token) 
{

}

void parse_attributes(Dom& dom, std::vector<Token> token)
{

}

