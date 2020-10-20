// could improve this by adding it to the main parser
// this could replace the other parser... we'll see

// TODO: create a new parser & maybe create this as a class?

#include <iostream>
#include <bitset>

#include "read_file.hpp"
#include "dom.hpp"

constexpr std::uint_fast8_t _tag  { 1 << 0 }; // 0000 0001 tag state
constexpr std::uint_fast8_t _attr { 1 << 1 }; // 0000 0010 attribute state
constexpr std::uint_fast8_t _temp { 1 << 2 }; // 0000 0100 template state
constexpr std::uint_fast8_t _cont { 1 << 3 }; // 0000 1000 content state

struct parserResult {
	// returning
	genericTag      tag = {};
	genericTemplate temp = {};
	std::string     out = "";
	Dom*            current_dom = nullptr;

	// tracking
	size_t*            col;
	const std::string* line;
};

typedef void parser(parserResult& result, std::uint_fast8_t& flag);
std::unordered_map<char, parser*> parser_map; // I guess it's like a state manager

bool is_letter(char character) 
{
	int c = character;

	if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
		return true;
	}

	return false;
}

// parse_tag reads a single line as a potential tag
// so many if statements...
void parse_tag(parserResult& result, std::uint_fast8_t& flag) 
{
	if(flag & (_attr | _temp)) return; // return if reading attribute or template

	size_t col = *result.col;
	std::string line = *result.line;

	col++;
	// return if the next character is not a letter or forward slash 
	if(col < line.size()) {
		char c = line[col];
		if(!is_letter(c) && c != '/') return;
	}

	flag |= _tag; // currently reading a tag
	size_t begin = col;
	bool found_end = false;
	parser* parse;

	// parse tag name
	for(; col < line.size(); col++)
	{
		char c = line[col];
		if (c == '/') {
			found_end = true;
		} else if(c == '>') {
			if(result.tag.name != "") 
			{
				if(found_end) {
					found_end = false;
					result.tag.is_single = true;
				}
				result.tag.is_start = true;
			}
			
			flag &= ~_tag;
		}

		if (c == ' ' || c == '>') 
		{
			result.tag.name = line.substr(begin, col);

			// parse tag attributes and/or template
			for(col++; col < line.size(); col++) 
			{
				if(parser_map.find(c) != parser_map.end()) {
					parse = parser_map[c];
					*result.col = col;
					result.line = &line.substr(col, line.size());
					parse(result, flag);
				}

				// TODO: figure out content
			}
		}
	}

	*result.col = col;
}

// parse_attr reads a single line as a potential attribute
void parse_attr(parserResult& result, std::uint_fast8_t& flag) 
{
	if (flag & (_temp | ~_tag)) return; // return if reading template or NOT reading tag

	size_t col = *result.col;
	std::string line = *result.line;

	flag |= _attr; // currently reading an attribute
	for(col++; col < line.size(); col++)
	{
		
	}
}

// parse_template reads a single line as a potential template
void parse_template(parserResult& result, std::uint_fast8_t& flag) 
{
	size_t col = *result.col;
	std::string line = *result.line;

	if(flag & _temp) 
	{
		for(col++; col < line.size(); col++)
		{
			char c = line[col];
			if(c == ' '){}
		}
	}

	flag |= _temp; // currently reading a template
}


std::uint_fast8_t parse_html(std::vector<std::string>& prev_line, std::string line, size_t row) // maybe return std::vector<parserResult> ?
{
	size_t col = 0;
	std::string content;
	std::uint_fast8_t flag{};
	parserResult result{};
	result.col = &col;
	result.line = &line;
	
	parser* parse;

	for(; col < line.size(); col++) {
		char& c = line[col];
		if(parser_map.find(c) != parser_map.end()) {
			parse = parser_map[c];

			result.line = &line.substr(col, line.size());
			parse(result, flag);
			content = result.out;
		}
	}

	return flag;
}

inline void create_template(std::vector<std::string> lines) // TODO: return something
{
	std::vector<std::string> prev_line; // for content context
	std::uint_fast8_t flag{}; // all states turned off to start

	for(size_t row = 0; row < lines.size(); row++)
	{
		std::string line = lines[row];

		flag = parse_html(prev_line, line, row);
		
	}
}

inline void init_parser() 
{
	// starting characters only
	// functions are required to 
	// handle stopping points
	parser_map['<'] = parse_tag;
	parser_map['='] = parse_attr;
	parser_map['{'] = parse_template;
}

inline Dom* parse_create_template(std::string path) //TODO: return something
{
	std::vector<std::string> lines;

	FileReader fr;
	std::cout << "reading template file: " << path << "\n\n";
	bool opened = fr.readFile(&lines, path.c_str());
	if (!opened)
	{
		std::cout << "failed to read file: " << path << "\n\n";
		return nullptr;
	}

	return nullptr;
}
