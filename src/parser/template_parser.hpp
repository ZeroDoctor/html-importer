// could improve this by adding it to the main parser
// this could replace the other parser... we'll see

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

#include "read_file.hpp"

struct templateContent 
{
	std::string content;
	size_t col = -1;
	size_t row = -1;
};

inline std::vector<templateContent> create_template(std::vector<std::string> lines) 
{

	std::vector<templateContent> result;
	templateContent temp_content;

	for(int i = 0; i < lines.size(); i++)
	{

		// TODO: create a new parser 
		std::string line = lines[i];

		size_t begin = line.find("{{", i);
		size_t end = line.find("}}", begin);
		std::string temp = line.substr(begin + 2, end - begin - 2);

		temp_content.content = temp;
		temp_content.col = begin;
		temp_content.row = i;
		result.push_back(temp_content);

		i = end;

		
	}

	return result;
}

inline std::vector<templateContent> parse_create_template(std::string path)
{
	std::vector<templateContent> template_contents;
	std::vector<std::string> lines;

	FileReader fr;
	std::cout << "reading template file: " << path << "\n\n";
	bool opened = fr.readFile(&lines, path.c_str());
	if (!opened)
	{
		std::cout << "failed to read file: " << path << "\n\n";
		return template_contents;
	}

	template_contents = create_template(lines);
	return template_contents;
}