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

void Dom::print_all()
{
	std::string content;
	std::cout << this->self.name << "\n\tchildren: " << this->get_children().size() << std::endl;

	for(auto attr : self.attrs) {
		std::cout << "\tAttr: " << attr.first << " Value: " << attr.second << std::endl;
	}

	if(this->get_content(content)) {
		std::cout << "\tContent: " << content << "\n";
	}

	std::vector<genericTemplate> temp;
	if (this->self.has_template(temp)) {
		for(auto t : temp) {
			std::cout << "\tID: " << t.id << "\tRow: " << t.row << "\tCol: " << t.col << std::endl;
		}
	}

	std::cout << "\n";
	
	for(auto child : this->get_children()) {
		child->print_all();
	}
}

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
		std::string cnt = remove_spaces(self.content);
		
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

void Dom::add_template(std::vector<genericTemplate> temp) 
{
	for (auto t : temp) {
		this->self.temp_contents.push_back(t);
	}
}

void Dom::set_temp_values(std::unordered_map<std::string, std::string> obj_map)
{
	std::queue<Dom *> q;
	std::vector<genericTemplate> gen_temp;
	q.push(this);

	while (!q.empty())
	{
		Dom *pgoal = q.front();
		q.pop();

		if(pgoal->self.has_template(gen_temp)) 
		{
			for(auto temp : gen_temp) {
				if(obj_map.find(temp.id) == obj_map.end()) continue;
				pgoal->self.content = obj_map[temp.id];
			}
		}

		for (auto child : pgoal->get_children())
			q.push(child);
	}
}

std::string Dom::get_file_name() { return file_name; }
void Dom::set_file_name(std::string file) { file_name = file; }

Dom::~Dom()
{
	std::cout << "removing " << this->get_name() << std::endl;
	for (auto child : this->children)
		delete (child);
}