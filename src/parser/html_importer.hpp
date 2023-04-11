#pragma once

#include <unordered_map>

#include "thread/thread_pool.hpp"
#include "dom.hpp"

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
