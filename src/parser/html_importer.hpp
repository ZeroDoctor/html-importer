#pragma once

#include <unordered_map>

#include "thread_pool.hpp"
#include "dom_parser.hpp"
#include "simdjson.h"

class HtmlImporter
{
private:
	std::vector<Dom *> m_doms;
	std::unordered_map<std::string, Dom *> m_files_load;

public:
	HtmlImporter(std::vector<Dom *> doms, std::unordered_map<std::string, Dom *>& files_load);
	~HtmlImporter();
	void Init();
};
