#include "utils.hpp"

void	utils::trim_str(std::string &str)
{
	size_t s, e;

	for (s = 0; str[s] == ' ' || str[s] == '\t'; s++);
	for (e = str.size() - 1; str[e] == ' ' || str[e] == '\t'; e--);
	str.erase(e + 1);
	str.erase(0, s);
}

void	utils::remove_comments(std::string &str)
{
	size_t s = str.find('#');
	if (s != std::string::npos)
		str.erase(s);
}
