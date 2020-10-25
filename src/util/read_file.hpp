#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

class FileReader
{

public:
	std::vector<std::string> getListFiles(const char* path)
	{
		std::vector<std::string> result;

		if (!std::filesystem::exists(path)) {
			return result;
		}

		for(const auto & entry : std::filesystem::directory_iterator(path)) 
		{
			if(entry.is_directory()) continue; // could recurse here

			std::string file = entry.path().string();

			if (file.find(".html") == std::string::npos) continue;

				result.push_back(file);
		}

		return result;
	}

	bool readFile(std::vector<std::string> *out_str, std::string file)
	{
		std::string line;
		std::ifstream infile(file);

		if (infile.is_open()) 
		{
			if (!infile) return false;

			while (std::getline(infile, line)) {
				out_str->push_back(line);
			}

			infile.close();
			
			return true;
		}

		return false;
	}
};