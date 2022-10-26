#include "parser/tokenizer.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "util/argh.h"
#include "parser/html_importer.hpp"

#define TSIZE 7

void usage() 
{
	std::cout << "usage: ihtml dst (dst = folders with *.html)" << std::endl;
}


int main(int argc, char** argv)
{
	spdlog::set_level(spdlog::level::debug);
	
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
		"<div>", 
			"whatever content in here", 
		"</div>"
	};

	Lexer lexer; 
	std::vector<Token> tokens = lexer.tokenizer(lines);

	for(auto token : tokens) {
		std::cout << token.to_string() << std::endl;
	}
	

	return 0;
}
