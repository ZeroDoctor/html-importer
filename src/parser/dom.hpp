#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>

struct attribute
{
	std::string name = "";
	std::string value = "";
};

struct genericTag
{
	std::string name = "";
	size_t line_number = -1;
	bool is_start = false;
	bool is_single = false;
	std::vector<attribute> attrs;
	std::string content = "";
};

class Dom
{

public:
	genericTag self;
	Dom *parent;
	std::vector<Dom *> children;
	std::size_t start_linenum = -1;
	std::size_t end_linenum = -1;

	Dom(genericTag s, Dom *p) : self(s), parent(p){};

	void add_child(Dom *d);

	Dom *find(std::string str);
	std::vector<Dom *> find_all(std::string str);

	std::string get_name();
	size_t get_line_number();
	std::vector<Dom *> get_children();
	bool get_attributes(std::vector<attribute> &attr);
	bool get_content(std::string &content);
	void add_content(std::string content);
	Dom *get_parent();

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

		if (pgoal->self.name == str)
			result.push_back(pgoal);
		for (auto child : pgoal->get_children())
			q.push(child);
	}

	return result;
}

std::string Dom::get_name() { return self.name; }
size_t Dom::get_line_number() { return self.line_number; }

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

bool Dom::get_attributes(std::vector<attribute> &attrs)
{
	try
	{
		if (self.attrs.size() > 1 && self.attrs[0].name == "")
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

void Dom::add_content(std::string content) {
	self.content += content;
}

Dom::~Dom()
{
	for (auto child : this->children)
		delete (child);
}