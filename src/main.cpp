#include <iostream>
#include <array>
#include <stack>
#include <chrono>

#include "read_file.hpp"
#include "thread_pool.hpp"
#include "pretty_console.hpp"
#include "dom_parser.hpp"
#include "argh.h"

#define TSIZE 7

namespace ptc = prettycon;

Dom* parse_create_dom(std::string path)
{
	std::vector<std::string> lines;
	FileReader fr;
	std::cout << "reading file: " << path << "\n\n";
	bool opened = fr.readFile(&lines, path.c_str());
	if (!opened) {
		std::cout << "failed to read file: " << path << "\n\n";
		return NULL;
	}
	return create_dom(lines, path);
}

void usage() 
{
	ptc::ccout.setColor(ptc::Color::black, ptc::Color::lyellow, ptc::Attribute::mydefault);
	std::cout << "usage: ihtml dst (dst = folders with *.html)" << std::endl;
	ptc::ccout.restore();
}

void print_template(std::string path, std::string file, std::string type, std::unordered_map<std::string, Dom*>& files_load) {
	if(file == "") {
		std::cout << "Error: file not found\n";
		return;
	}

	std::filesystem::path root(path);
	std::string src = root.parent_path().string() + "/" + file;

	Dom* dom;
	if(files_load.find(src) == files_load.end()) {
		dom = parse_create_dom(src);
		files_load[src] = dom;
	} else {
		dom = files_load[src];
	}
	
	auto tags = dom->find_all("div");
	for(auto tag : tags)
	{	
		std::unordered_map<std::string, std::string> v_attr;
		bool has_attr = tag->get_attributes(v_attr);
		if(has_attr && type == v_attr["type"]) {
			std::string output = type + " / " + v_attr["type"];
			std::cout << "div of type " + output + " start from: "<< tag->start_linenum << " to " << tag->end_linenum << std::endl;
		}
	}
}

void process_include(std::vector<Dom*> dom, std::string path, std::unordered_map<std::string, Dom*>& files_load) {
	if (dom.size() > 0)
	{
		for (auto tag : dom)
		{
			std::unordered_map<std::string, std::string> v_attr;
			std::string content;

			bool has_attr = tag->get_attributes(v_attr);
			if(has_attr) 
			{
				bool has_content = tag->get_content(content);
				if(!has_content) continue;

				std::string file = v_attr["path"];
				std::string type = v_attr["type"];

				print_template(path, file, type, files_load);
			}
		}
	}
}


int main(int argc, char** argv)
{
	auto cmdl = argh::parser(argc, argv);

	const std::string dst = cmdl[1];
	if(dst == "") {
		usage();
		return 0;
	}

	FileReader fr;
	std::vector<std::string> files = fr.getListFiles((std::string("./")+dst).c_str());
	for (auto file : files) {
		std::cout << file << std::endl;
	}

	ThreadPool pool(TSIZE);

	pool.init();
	auto future = std::array<std::future<Dom*>, TSIZE>();
	std::unordered_map<std::string, Dom*> files_load;
	std::vector<Dom*> doms;

	ptc::ccout.setColor(ptc::Color::black, ptc::Color::lgreen, ptc::Attribute::mydefault);

	int index = 0;
	for(int i = 0; i < files.size(); i++) 
	{
		index = i % TSIZE;
		if(future[index].valid()) // could be alot better but its good enough for now
		{ 
			auto status = future[index].wait_for(std::chrono::seconds(0));
			while(status != std::future_status::ready) {
				std::cout << "waiting for future[" << index << "] to return" << std::endl;
				status = future[index].wait_for(std::chrono::milliseconds(500));
			}

			doms.push_back(future[index].get());
			
		}

		future[index] = pool.submit(parse_create_dom, files[i]);
	}

	for(int i = 0; i < TSIZE; i++) {
		if(future[i].valid()) {
			doms.push_back(future[index].get());
		}
	}

	std::cout << "processing..." << std::endl;
	for(auto dom : doms) {
		files_load[dom->get_file_name()] = dom;
		process_include(dom->find_all("include"), dom->get_file_name(), files_load); // needs to be async?
	}

	pool.shutdown();
	ptc::ccout.restore();
	std::cout << "Done!" << std::endl;

	return 0;
}
