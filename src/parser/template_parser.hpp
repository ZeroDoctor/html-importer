// ? maybe create this as a class?
// * NOTE: single attributes will be lost 
// * i.e. the 'data-body' in <body data-body> will not appear in dom

#include <iostream>
#include <bitset>

#include "read_file.hpp"
#include "dom.hpp"

#define SET_FLAG(n, f) ((n) |= (f))
#define CLR_FLAG(n, f) ((n) &= ~(f))
#define TGL_FLAG(n, f) ((n) ^= (f))
#define CHK_FLAG(n, f) ((n) & (f))

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
inline std::unordered_map<char, parser*> parser_map; // I guess it's like a state manager

inline bool is_letter(char character) 
{
	int c = character;

	if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
		return true;
	}

	return false;
}

// parse_tag reads a single line as a potential tag
// so many if statements...
inline void parse_tag(parserResult& result, std::uint_fast8_t& flag) 
{
	if(flag & (_attr | _temp)) return; // return if reading attribute or template

	size_t col = *result.col;
	std::string line = *result.line;

	col++;
	// return if the next character is not a letter or forward slash 
	if(col < line.size()) {
		char c = line[col];
		if(!is_letter(c) && c != '/') {
			return;
		}
	}

	SET_FLAG(flag, _tag); // currently reading a tag
	size_t begin_name = col;
	size_t end_name = col;
	bool found_end = false;
	parser* parse;

	// parse tag name
	for(; col < line.size(); col++)
	{
		char c = line[col];
		if (c == '/') {
			found_end = true;
		} else if(c == '>') {
			if(result.tag.name != "" && found_end) {
				found_end = false;
				if(line[col-1] == '/') result.tag.is_single = true; // could break the program
			}

			result.tag.name = line.substr(begin_name, end_name-begin_name);
			if(!found_end) result.tag.is_start = true;
			CLR_FLAG(flag, _tag);
			break;
		}

		if (result.tag.name == "" && 
			  (std::isspace<char>(c, std::locale::classic()) || col+1 == line.size())) {

            if (col+1 == line.size()) col++; // for '>' found on a new line
			result.tag.name = line.substr(begin_name, col-begin_name);
			end_name = col;
			result.tag.is_start = true;

		} else if(result.tag.name != "" && !found_end) {
			// parse attribute/template info
			if (parser_map.find(c) != parser_map.end())
			{
				parse = parser_map[c];
				*result.col = col;
				parse(result, flag);
				col = *result.col;
			}
		} else {
			end_name = col+1;
		}
	}

	std::cout << "result: " << result.tag.name << " " << end_name << ", " << begin_name << std::endl;
	*result.col = col;
}

// parse_attr reads a single line as a potential attribute
inline void parse_attr(parserResult& result, std::uint_fast8_t& flag)
{
	if (flag & _temp && (flag & _tag) == 0) return; // return if reading template or NOT reading tag

	size_t col = *result.col;
	std::string line = *result.line;

	char curr_quote = ' ';

	size_t name_start = line.rfind(' ', col); // cheated a bit
	std::string first_attr = line.substr(name_start, col-name_start);

	SET_FLAG(flag, _attr); // currently reading an attribute
	for(col++; col < line.size(); col++)
	{
		char c = line[col];
		if(c == '"' || c == '\'')
		{
			if(curr_quote == ' ') {
				curr_quote = c;
			} else if(c == curr_quote) {
				curr_quote = ' ';
			}
		}

		if(curr_quote == ' ' && (c == '/' || c == '>'))
		{
			col--; // let parser_tag handle '/' and '>'
			CLR_FLAG(flag, _attr);
			break;
		}
	}

	*result.col = col;
}

// parse_template reads a single line as a potential template
inline void parse_template(parserResult& result, std::uint_fast8_t& flag) 
{
	return;
	size_t col = *result.col;
	std::string line = *result.line;

	if(flag & _temp) 
	{
		for(col++; col < line.size(); col++)
		{
			char c = line[col];
		}
	}

	SET_FLAG(flag, _temp); // currently reading a template
}


inline std::uint_fast8_t parse_html(std::vector<std::string>& prev_line, std::string line, parserResult& result, size_t row) // maybe return std::vector<parserResult> ?
{
	size_t col = 0;
	std::string temp_line;
	std::uint_fast8_t flag{};
	result.col = &col;
	result.line = &line;
	
	parser* parse;

	for(; col < line.size(); col++) {
		char& c = line[col];
		if(parser_map.find(c) != parser_map.end()) {
			parse = parser_map[c];
			parse(result, flag);
			std::cout << std::endl;
		} else {
			// content is here
			// TODO: buffer '{' before adding it to content
		}
	}

	std::cout << std::endl;	

	return flag;
}

inline void create_template(std::vector<std::string> lines) // TODO: return something
{
	std::cout << "creating template...\n";
	std::vector<std::string> prev_line; // for content context
	std::uint_fast8_t flag{}; // all states turned off to start
	parserResult result{};
	std::vector<parserResult> results;

	for(size_t row = 0; row < lines.size(); row++)
	{
		std::string line = lines[row];

		flag = parse_html(prev_line, line, result, row);
		if(!(flag & _tag)) {
			results.push_back(result);
			result = {};
		}
	}
}

inline void init_parser() 
{
	// starting characters only
	// functions are required to 
	// handle stopping points
	parser_map['<'] = &parse_tag; // * done implementing for the most part (comments handler not done)
	parser_map['='] = &parse_attr;
	parser_map['#'] = &parse_template;
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

	create_template(lines);

	return nullptr;
}
