// HtmlImporter.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <array>
#include <stack>

#include "read_file.hpp"
#include "thread_pool.hpp"
#include "pretty_console.hpp"
#include "parser.hpp"
#include "argh.h"

namespace ppt = prettycon;

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
	return create_dom(lines);
}

void usage() {
	ppt::ccout.setColor(ppt::Color::black, ppt::Color::lyellow, ppt::Attribute::mydefault);
	std::cout << "usage: ihtml dst (dst = folders with *.html)" << std::endl;
	ppt::ccout.restore();
}

void printTemplate(std::string path, std::string file) {
	std::filesystem::path root(path);
	std::string src = root.parent_path().string() + "/" + file;


	FileReader fr;
	std::vector<std::string> lines;
	bool opened = fr.readFile(&lines, src.c_str());
	
	if(opened) {
		for (auto l : lines) {
			std::cout << l << std::endl;
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

	ThreadPool pool(4);

	pool.init();
	auto future = std::array<std::future<Dom*>, 5 * 3>();

	future[0] = pool.submit(parse_create_dom, files[0]);

	ppt::ccout.setColor(ppt::Color::black, ppt::Color::lgreen, ppt::Attribute::mydefault);
	
	// files[0].html
	std::vector<Dom*> atag = future[0].get()->find_all("include");
	if (atag.size() > 0)
	{
		for (auto tag : atag)
		{
			std::vector<attribute> v_attr;

			std::cout << "tag name: " << tag->get_name() << std::endl;
			tag->get_attributes(v_attr);
			for (auto attr : v_attr)
			{
				std::cout << "\t" << attr.name << " : " << attr.value << " | " << tag->get_line_number() << std::endl;
				printTemplate(files[0], attr.value);
			}
		}
	}

	pool.shutdown();
	ppt::ccout.restore();
	std::cout << "Done!" << std::endl;

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
