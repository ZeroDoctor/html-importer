#include <array>
#include <stack>
#include <chrono>

#include "util/pretty_console.hpp"
#include "util/argh.h"
#include "parser/html_importer.hpp"

#define TSIZE 7

void usage() 
{
	std::cout << "usage: ihtml dst (dst = folders with *.html)" << std::endl;
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
	init_parser();

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

		future[index] = pool.submit(parse_create_template, files[i]);
	}

	for(int i = 0; i < TSIZE; i++) {
		if(future[i].valid()) {
			std::cout << "created dom..." << std::endl;
			doms.push_back(future[index].get());
		}
	}

	std::cout << "importing..." << std::endl;
	HtmlImporter include(doms, files_load);
	include.Init(); 

	pool.shutdown();
	std::cout << "Done!" << std::endl << std::endl;

	for(auto d : doms) {
		if(d != nullptr) {
			std::cout << "deleting " + d->get_name() + " dom..." << std::endl;
			delete d;
		}
	}

	return 0;
}
