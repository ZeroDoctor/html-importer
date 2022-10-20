#include <array>
#include <stack>
#include <chrono>

#include "spdlog/spdlog.h"
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
		spdlog::info("loaded [file={}]", dst);
	}


	return 0;
}
