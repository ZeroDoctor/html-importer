#include <iostream>
#include <array>
#include <stack>
#include <chrono>

#include "thread_pool.hpp"
#include "pretty_console.hpp"
#include "html_importer.hpp"
#include "argh.h"

#define TSIZE 7

namespace ptc = prettycon;

void usage() 
{
	ptc::ccout.setColor(ptc::Color::black, ptc::Color::lyellow, ptc::Attribute::mydefault);
	std::cout << "usage: ihtml dst (dst = folders with *.html)" << std::endl;
	ptc::ccout.restore();
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

	HtmlImporter importer(doms, files_load);
	importer.Init();

	pool.shutdown();
	ptc::ccout.restore();
	std::cout << "Done!" << std::endl;

	return 0;
}
