#include "dom.hpp"

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
		{
			result.push_back(pgoal);
		}

		for (auto child : pgoal->get_children())
			q.push(child);
	}

	return result;
}

void Dom::add_child(Dom *dom) { children.push_back(dom); }

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

Dom *Dom::get_parent() { return parent; }

std::string Dom::get_name() { return self.name; }
size_t Dom::start_tag_line() { return self.start_linenum; }
size_t Dom::end_tag_line() { return self.end_linenum; }
void Dom::set_end_tag_line(size_t end) { self.end_linenum = end; }

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