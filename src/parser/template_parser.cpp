#include "template_parser.hpp"

void init_parser()
{
    // starting characters only
	// functions are required to 
	// handle stopping points
	parser_map['<'] = &parse_tag; // * done implementing for the most part (comments handler not done)
	parser_map['='] = &parse_attr; // * done with some issues
	parser_map['#'] = &parse_template; // * done
}

void parse_tag(parserResult& result, std::uint_fast8_t& flag)
{
    if(flag & (_attr | _temp)) return; // return if reading attribute or template

	size_t col = *result.col;
	std::string line = *result.line;

	col++;
	// return if the next character is not a letter or forward slash 
	if(col < line.size()) 
	{
		char c = line[col];
		if(!is_letter(c) && c != '/') {
			col--;
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
		if (c == '/') { // check if potential end
			found_end = true;
		} else if(c == '>') { // exit loop and set tag info like name, tag type, and if tag is single
			if(result.tag.name != "" && found_end) {
				found_end = false;
				if(line[col-1] == '/') result.tag.is_single = true; // could break the program
			}

			if (!result.tag.is_single) {
				result.tag.name = line.substr(begin_name, end_name-begin_name);
			}
			
			if(!found_end) result.tag.is_start = true;
			CLR_FLAG(flag, _tag);
			break;
		}

		if (result.tag.name == "" && // store tag name if space or new line is found
			  (std::isspace<char>(c, std::locale::classic()) || col+1 == line.size())) 
		{
            if (col+1 == line.size()) col++; // for '>' found on a new line
			result.tag.name = line.substr(begin_name, col-begin_name);
			end_name = col;
			result.tag.is_start = true;

		} else if(result.tag.name != "" && !found_end) { // since name is found, find attributes
			// parse attribute/template info
			if (parser_map.find(c) != parser_map.end())
			{
				parse = parser_map[c];
				*result.col = col;
				parse(result, flag);
				col = *result.col;

				result.temp_bool = false;
			}
		} else {
			end_name = col+1; // a hack for tag without attributes
		}
	}

	*result.col = col;
}

void parse_attr(parserResult& result, std::uint_fast8_t& flag)
{
    if (flag & _temp || !(flag & _tag)) return; // return if reading template or NOT reading tag

	size_t col = *result.col;
	std::string line = *result.line;

	char curr_quote = ' ';

	size_t name_start = line.rfind(' ', col); // cheated a bit
	std::string attr = line.substr(name_start, col-name_start);
	std::string value = "";
	parser* parse;

	SET_FLAG(flag, _attr); // currently reading an attribute
	for(col++; col < line.size(); col++)
	{
		char c = line[col];
		if(c == '"' || c == '\'') // find if inside quotes
		{
			if(curr_quote == ' ') {
				curr_quote = c;
			} else if(c == curr_quote) {
				curr_quote = ' ';
			}
		}

		if(curr_quote == ' ' && (c == '/' || c == '>')) // exit loop at tag ends
		{
			col--; // let parser_tag handle '/' and '>'
			CLR_FLAG(flag, _attr);
			break;
		}

		if(curr_quote != ' ' && c != curr_quote) { // find values and attr names to store inside tag
			value += c;
		} else if(value != "") {
			result.tag.attrs[attr] = value;
			attr = "";
			value = "";
		} 
		
		if(c == '=') {
			name_start = line.rfind(' ', col); // cheated again
			attr = line.substr(name_start, col-name_start);
		}

		if (parser_map.find(c) != parser_map.end()) // find templates
		{
			parse = parser_map[c];
			*result.col = col;
			parse(result, flag);
			col = *result.col;

			result.temp_bool = false;
		}
	}

	*result.col = col;
}

void parse_template(parserResult& result, std::uint_fast8_t& flag)
{
    size_t col = *result.col;
	std::string line = *result.line;

	if(col-1 >= 0 && line[col-1] != '{') {return;}

	std::string id;
	size_t begin = col;

	SET_FLAG(flag, _temp); // currently reading a template
	for(col++; col < line.size(); col++)
	{
		char c = line[col];
		if(c == ' ') return; // no spaces in template ids
		if(c == '}') break;
		id += c;
	}

	genericTemplate temp = {};
	temp.id = id;
	temp.row = *result.row;
	temp.col = begin;
	result.tag.temp_contents.push_back(temp);
	result.temp_bool = true;
	CLR_FLAG(flag, _temp);

	*result.col = col;
}

std::uint_fast8_t parse_html(
        std::string line, std::vector<parserResult>& results, 
        size_t row, std::string& content
    )
{
    size_t col = 0;
	std::string temp_line;
	std::uint_fast8_t flag{};
	parserResult result{};
	result.row = &row;
	result.col = &col;
	result.line = &line;
	
	parser* parse;
	size_t loc = 0;
	std::vector<genericTemplate> g_temp;
	bool temp_bool = false;

	for(; col < line.size(); col++)
	{
		char& c = line[col];
		if(parser_map.find(c) != parser_map.end()) {
			parse = parser_map[c];
			parse(result, flag);
			if(!(flag & _tag))
			{
				result.tag.row = row;
				result.tag.col = col;
				results.push_back(result);
				
				temp_bool = result.temp_bool;

				result = {};
				result.line = &line;
				result.col = &col;
				result.row = &row;

			}
		} else {
			content += c;
		}

		if(temp_bool) {
			std::cout << "-- erasing {" << std::endl;
			content.erase(content.end()-1);
			temp_bool = false;
		}
	}

	return flag;
}

void create_template(std::vector<std::string> lines)
{
    std::cout << "creating template...\n";
	std::uint_fast8_t flag{}; // all states turned off to start
	std::vector<parserResult> results;
	std::vector<genericTemplate> g_temp;
    std::stack<std::string> content_stack;
	std::string content;
	
	bool temp_bool = false;
	std::vector<genericTemplate> temp;

	std::stack<Dom *> dom;
	Dom* current_dom = nullptr;
	Dom* prev_dom = nullptr;
	Dom* root = nullptr;

	dom.push(nullptr);

	for(size_t row = 0; row < lines.size(); row++)
	{
		std::string line = lines[row];
		parse_html(line, results, row, content);
		
		for(size_t i = 0; i < results.size(); i++)
		{
			parserResult r = results[i];
			temp_bool = r.temp_bool;
			if(temp_bool) temp = r.tag.temp_contents;
			if (r.tag.name == "") continue;

			prev_dom = dom.top();
			if(r.tag.is_start)
			{
				current_dom = new Dom(r.tag, prev_dom);
				if(prev_dom != nullptr) {
					prev_dom->add_child(current_dom);
				}
				if(!r.tag.is_single) {
					dom.push(current_dom);
				} else {
					root = current_dom;
				}

                if(remove_spaces(content) != "") {
					std::cout << "content push: " << content << std::endl;
                    content_stack.push(content);
                }
			} else if(prev_dom != nullptr && ("/" + prev_dom->get_name()) == r.tag.name) {
				prev_dom->end_linenum = row;

				if(current_dom != nullptr) {
					if(content_stack.size() <= 0) {
						prev_dom->add_content(content);
						content = "";
					} else {
						content = content_stack.top();
						content_stack.pop();
					}

					if(temp.size() > 0) {
						current_dom->add_template(temp);
						temp.clear();
					}
				}

				root = dom.top();
				dom.pop();
			} else if (prev_dom != nullptr) {
				std::cout << "missing end tag... " + prev_dom->get_name() << std::endl;
			}
		}

		results.clear();
	}

	root->print_all();
}

Dom* parse_create_template(std::string path)
{
    std::vector<std::string> lines;

	FileReader fr;
	std::cout << "reading template file: " << path << "\n\n";
	bool opened = fr.readFile(&lines, path.c_str());
	if (!opened) {
		std::cout << "failed to read file: " << path << "\n\n";
		return nullptr;
	}

	create_template(lines);

	return nullptr;
}