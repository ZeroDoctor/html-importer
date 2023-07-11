#include "parser/tokenizer.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "util/argh.h"
#include "parser/html_importer.hpp"
#include "util/read_file.hpp"

#define TSIZE 7

void usage() 
{
	std::cout << "usage: ihtml dst (dst = folders with *.html)" << std::endl;
}


int main(int argc, char** argv)
{
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("start program...");
	
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

	std::vector<std::string> lines = {
		" this should be content",
		"<!-- the knew!! -->", 
		"<image src='https://img.zerodoc.dev/v1/test'/>",
		"<image />",
		"<div attr='another one, but was there?'>",
			"whatever content in here", 
			"<div inner='woowowowo'>", 
				"this is child content",
			"</div>", 
			"more content in here <melee>luigi there is nothing like doing nothing</melee>", 
			"and even more content in here", 
		"</div>"
	};

	Lexer lexer; 
	std::vector<Token> tokens = lexer.tokenizer(lines);

	for(auto token : tokens) {
		spdlog::debug("{}", token.to_string());
	}
	
	spdlog::info("end program...");
	return 0;
}
