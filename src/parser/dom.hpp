#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

struct attribute
{
	std::string name = "";
	std::string value = "";
};

struct genericTag
{
	std::string name = "";
	size_t start_linenum = -1;
	size_t end_linenum = -1;
	bool is_start = false;
	bool is_single = false;
	std::unordered_map<std::string, std::string> attrs;	
	std::string content = "";
};

class Dom
{
private:
	Dom *parent;
	std::vector<Dom *> children;
	genericTag self;
	std::string file_name;

public:	
	std::size_t start_linenum = -1;
	std::size_t end_linenum = -1;

	Dom(genericTag s, Dom *p) : self(s), parent(p){};

	Dom *find(std::string str);
	std::vector<Dom *> find_all(std::string str);
	void add_child(Dom *d);
	std::vector<Dom *> get_children();
	Dom *get_parent();

	std::string get_name();
	size_t start_tag_line();
	size_t end_tag_line();
	void set_end_tag_line(size_t end);
	
	bool get_attributes(std::unordered_map<std::string, std::string> &attr);
	bool get_content(std::string &content);
	void add_content(std::string content);
	
	void set_file_name(std::string file);
	std::string get_file_name();

	~Dom();

};
