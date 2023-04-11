#include "html_importer.hpp"

namespace sdom = simdjson::dom;

void output() {

}

Dom* include_component() {

	Dom* new_dom = nullptr;

	// TODO: replace include tag with desired div tag type

	return new_dom;
}

void process_component(
	std::string src, std::string type, std::vector<sdom::object> objects,
	std::unordered_map<std::string, Dom *> &files_load
)
{
	Dom *dom;
	if (files_load.find(src) == files_load.end()) {
		dom = nullptr; //parse_create_template(src);
		if(dom == nullptr) return;
		files_load[src] = dom;
	} else {
		dom = files_load[src];
	}

	std::unordered_map<std::string, std::string> obj_map;
	for (auto obj : objects) {
		for (auto [key, value] : obj) {
			obj_map[std::string(key)] = std::string(value);
		}
	}

	auto tags = dom->find_all("component");
	for (auto tag : tags)
	{
		std::unordered_map<std::string, std::string> v_attr;
		bool has_attr = tag->get_attributes(v_attr);
		if (has_attr && type == v_attr["type"])
		{
			tag->set_temp_values(obj_map);
			tag->print_all();
			std::cout << "--------------\n";
		}
	}
}

void parse_json(std::vector<sdom::object>& result, sdom::element element)
{
	switch(element.type()) {
		case sdom::element_type::ARRAY:
			for(sdom::element child : sdom::array(element)) {
				parse_json(result, child);
			}
		break;
		case sdom::element_type::OBJECT:
			result.push_back(sdom::object(element));	
		break;

		default:
			std::cout << "--! Failed to parse element" << std::endl;
	}
}

void process_include(Dom *dom, std::unordered_map<std::string, Dom *> &files_load)
{
	std::vector<Dom *> doms = dom->find_all("include");
	std::string path = dom->get_file_name();

	for (auto tag : doms)
	{
		std::unordered_map<std::string, std::string> v_attr;
		std::string content;

		bool has_attr = tag->get_attributes(v_attr);
		if (has_attr)
		{
			bool has_content = tag->get_content(content);

			std::string file = v_attr["path"];
			if(file == "") 
			{
				std::cerr << "Error include: failed to find file path: " << file + "\n";
				return;
			}

			if (v_attr.find("type") == v_attr.end()) 
			{
				std::cerr << "Error include [<" << tag->start_linenum << "]: failed to find type: " << file + "\n";
				return;
			}
			std::string type = v_attr["type"]; 

			if (has_content) 
			{
				sdom::parser parser;
				sdom::element elem;

				auto error = parser.parse(content).get(elem);
				if (error)
				{
					std::cerr << error << std::endl;
					return;
				}
				std::vector<sdom::object> objects;
				parse_json(objects, elem);

				// std::filesystem::path root(path);
				std::string src = "./" + file;

				process_component(src, type, objects, files_load);
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

HtmlImporter::~HtmlImporter() {}
