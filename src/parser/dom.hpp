#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <locale>

// could remove this struct
struct attribute
{
	std::string name = "";
	std::string value = "";
};

struct genericTag
{
	std::string name = "";
	bool is_start = false;
	bool is_single = false;
	std::unordered_map<std::string, std::string> attrs;
	
	std::string content = ""; // imagine content taking up the whole std::string... crazy, not impossible but... still crazy
	std::vector<std::string> template_contents; // content used for passing values to template

	bool is_empty() {
		if(name == "") return true;
		return false;
	}
};

struct genericTemplate
{
	std::string name = "";
	size_t start_linenum = size_t(-1);
	size_t end_linenum = size_t(-1);

	bool is_empty() {
		if(name == "") return true;
		return false;
	}
};

inline std::string remove_spaces(std::string str) {
	str.erase(
		std::remove_if(str.begin(), str.end(),
			[](char &c) {
				return std::isspace<char>(c, std::locale::classic());
			}),
		str.end());

	return str;
}

class Dom
{
private:
	genericTag self;
	Dom *parent;
	std::vector<Dom *> children;

	// only root
	std::string file_name;

public:	
	size_t start_linenum = -1;
	size_t end_linenum = -1;

	Dom(genericTag s, Dom *p) : self(s), parent(p){};

	Dom *find(std::string str);
	std::vector<Dom *> find_all(std::string str);
	void add_child(Dom *d);
	std::vector<Dom *> get_children();
	Dom *get_parent();

	std::string get_name();
	
	bool get_attributes(std::unordered_map<std::string, std::string> &attr);
	bool get_content(std::string &content);
	void add_content(std::string content);
	std::vector<Dom*> find_all_templates(); // not implemented yet

	
	// only root
	void set_file_name(std::string file);
	std::string get_file_name();

	~Dom();

};
