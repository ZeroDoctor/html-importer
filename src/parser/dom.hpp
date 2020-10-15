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

void Dom::add_child(Dom *dom) { children.push_back(dom); }

Dom *Dom::find(std::string str)
{
	std::queue<Dom *> q;
	q.push(this);
	while (!q.empty())
	{
		Dom *pgoal = q.front();
		q.pop();

		if (pgoal->self.name == str)
			return pgoal;
		for (auto child : pgoal->get_children())
			q.push(child);
	}

	return NULL;
}

std::vector<Dom *> Dom::find_all(std::string str)
{
	std::vector<Dom *> result;

	std::queue<Dom *> q;
	q.push(this);
	while (!q.empty())
	{
		Dom *pgoal = q.front();
		q.pop();

		if (pgoal->self.name == str) {
			result.push_back(pgoal);
		}
			
		for (auto child : pgoal->get_children())
			q.push(child);
	}

	return result;
}

std::string Dom::get_name() { return self.name; }
size_t Dom::start_tag_line() { return self.start_linenum; }
size_t Dom::end_tag_line() { return self.end_linenum; }
void Dom::set_end_tag_line(size_t end) {self.end_linenum = end; }

std::vector<Dom *> Dom::get_children()
{
	std::vector<Dom *> result;

	try
	{
		result = children;
	}
	catch (std::exception &e)
	{
		std::cout << "Exception: " << e.what();
	}

	return result;
}

Dom* Dom::get_parent() { return parent; }

bool Dom::get_attributes(std::unordered_map<std::string, std::string> &attrs)
{
	try
	{
		if (self.attrs.size() <= 0)
			return false;

		attrs = self.attrs;
	}
	catch (std::exception &e)
	{
		std::cout << "Exception: " << e.what();
		return false;
	}

	return true;
}

bool Dom::get_content(std::string &content)
{
	try
	{
		if (self.content != "")
		{
			content = self.content;
			return true;
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Exception: " << e.what();
		return false;
	}

	return false;
}

void Dom::add_content(std::string content) { self.content += content; }

std::string Dom::get_file_name() { return file_name; }
void Dom::set_file_name(std::string file) { file_name = file; }

Dom::~Dom()
{
	for (auto child : this->children)
		delete (child);
}