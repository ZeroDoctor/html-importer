#pragma once

#include <iostream>

#include "read_file.hpp"
#include "dom_parser.hpp"

class HtmlImporter
{
private:
	std::vector<Dom *> m_doms;
	std::unordered_map<std::string, Dom *> m_files_load;

public:
	HtmlImporter(std::vector<Dom *> doms, std::unordered_map<std::string, Dom *> & files_load);
	~HtmlImporter();
	void Init();
};

