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
		// remove white space
		std::string cnt = self.content;
		cnt.erase(
			std::remove_if(cnt.begin(), cnt.end(), 
				[](char &c) {
					return std::isspace<char>(c, std::locale::classic());
				}
			), cnt.end());
		
		// check if valid content
		if (cnt != "")
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

std::vector<std::string> Dom::get_template_content() { return self.template_contents; }

void Dom::parse_template_content(std::string content) 
{
	size_t begin = content.find("{{", 0);
	size_t end = 0;
	while (begin != std::string::npos) {
		std::cout << start_linenum << " : " << end_linenum << std::endl;
		end = content.find("}}", begin);
		std::string test = content.substr(begin, end+2);
		self.template_contents.push_back(test);
		begin = content.find("{{", begin+1);
	}
}

std::string Dom::get_file_name() { return file_name; }
void Dom::set_file_name(std::string file) { file_name = file; }

Dom::~Dom()
{
	for (auto child : this->children)
		delete (child);
}