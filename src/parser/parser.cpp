#include "parser.hpp"
#include "parser/tokenizer.hpp"
#include <memory>

std::unique_ptr<Dom> parse_tokens(std::vector<Token> tokens)
{
	Tag rootTag = {};
	Dom* root = new Dom(rootTag, nullptr);

	// create stack
	std::stack<Dom*> dom_stack;
	dom_stack.push(root);

	for(size_t i = 0; i < tokens.size(); i++) {
		Dom* parent = dom_stack.top();
		dom_stack.pop();

		switch(tokens[i].type) {
			case TOKEN_CONTENT:
				i = parse_content(*parent, tokens, i);
			break;
			case TOKEN_TAG_LESS:
			case TOKEN_TAG_LESS_DASH:
			case TOKEN_TAG_GREATER:
			case TOKEN_TAG_GREATER_DASH:
			case TOKEN_TAG_NAME:
			case TOKEN_ATTRIBUTE_NAME:
			case TOKEN_ATTRIBUTE_EQUAL:
			case TOKEN_ATTRIBUTE_VALUE:
			case TOKEN_ATTRIBUTE_VALUE_START:
			case TOKEN_ATTRIBUTE_VALUE_END:
				i = parse_tag(parent, tokens, i);
				
				// if end tag is found
				// grab top of stack and append children
			break;
			case TOKEN_COMMENT_LESS:
			case TOKEN_COMMENT_GREATER:
			case TOKEN_COMMENT_CONTENT:
				// do nothing...
			break;
		}

	}

	return std::move(std::unique_ptr<Dom>(root));
}

size_t parse_content(Dom& dom, std::vector<Token> tokens, size_t index)
{
	dom.get_self().content = tokens[index].text;
	
	return index;
}

size_t parse_tag(Dom* dom, std::vector<Token> tokens, size_t index) 
{
	switch(tokens[index].type) {
		case TOKEN_ATTRIBUTE_NAME:
		case TOKEN_ATTRIBUTE_EQUAL:
		case TOKEN_ATTRIBUTE_VALUE:
		case TOKEN_ATTRIBUTE_VALUE_START:
		case TOKEN_ATTRIBUTE_VALUE_END:
			return parse_attributes(*dom, tokens, index);
		break;
		case TOKEN_TAG_LESS:
		case TOKEN_TAG_GREATER:
		case TOKEN_TAG_LESS_DASH:
		case TOKEN_TAG_GREATER_DASH:
		default: break;
	}
	
	Tag childTag = {};
	childTag.name = tokens[index].text;
	childTag.row = tokens[index].start_row;
	childTag.col = tokens[index].start_col;
	Dom* child = new Dom(childTag, dom);
	dom->add_child(child);

	return index;
}

size_t parse_attributes(Dom& dom, std::vector<Token> tokens, size_t index)
{
	return index;
}

