#include "html_importer.hpp"

void print_template(std::string path, std::string file, std::string type, std::unordered_map<std::string, Dom *> &files_load)
{
	if (file == "")
	{
		std::cout << "Error: file not found\n";
		return;
	}

	std::filesystem::path root(path);
	std::string src = root.parent_path().string() + "/" + file;

	Dom *dom;
	if (files_load.find(src) == files_load.end())
	{
		dom = parse_create_dom(src);
		files_load[src] = dom;
	}
	else
	{
		dom = files_load[src];
	}

	auto tags = dom->find_all("div");
	for (auto tag : tags)
	{
		std::unordered_map<std::string, std::string> v_attr;
		bool has_attr = tag->get_attributes(v_attr);
		if (has_attr && type == v_attr["type"])
		{
			std::string output = type + " / " + v_attr["type"];
			std::cout << "div of type " + output + " start from: " << tag->start_linenum << " to " << tag->end_linenum << std::endl;
		}
	}
}

void process_include(Dom *dom, std::unordered_map<std::string, Dom *> &files_load)
{

	std::vector<Dom *> doms = dom->find_all("include");
	std::string path = dom->get_file_name();

	if (doms.size() > 0)
	{
		for (auto tag : doms)
		{
			std::unordered_map<std::string, std::string> v_attr;
			std::string content;

			bool has_attr = tag->get_attributes(v_attr);
			if (has_attr)
			{
				bool has_content = tag->get_content(content);
				if (!has_content)
					continue;

				std::string file = v_attr["path"];
				std::string type = v_attr["type"];

				simdjson::dom::parser parser;
				simdjson::dom::object object;
				auto error = parser.parse(content).get(object);
				if (error)
				{
					std::cerr << error << std::endl;
					return;
				}
				for (auto [key, value] : object)
				{
					std::cout << key << " = " << value << std::endl;
				}

				//print_template(path, file, type, files_load);
			}
		}
	}
}

HtmlImporter::HtmlImporter(std::vector<Dom *> doms, std::unordered_map<std::string, Dom *>& files_load)
{ 
	m_doms = doms;
	m_files_load = files_load;
}

void HtmlImporter::Init()
{
	// ? should I make a class
	std::cout << "processing..." << std::endl;
	for (auto dom : m_doms)
	{
		m_files_load[dom->get_file_name()] = dom;
		process_include(dom, m_files_load); // needs to be async?
	}
}

HtmlImporter::~HtmlImporter()
{
}