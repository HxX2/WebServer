#include "utils.hpp"

void utils::remove_comments(std::string &str)
{
	size_t s = str.find('#');
	if (s != std::string::npos)
		str.erase(s);
}

void utils::trim_str(std::string &str)
{
	size_t s, e;

	for (s = 0; str[s] == ' ' || str[s] == '\t'; s++)
		;
	for (e = str.size() - 1; str[e] == ' ' || str[e] == '\t'; e--)
		;
	str.erase(e + 1);
	str.erase(0, s);
	if (!str.empty() && str.at(str.size() - 1) == ';')
		str.erase(str.size() - 1);
}

std::vector<std::string> utils::split_str(std::string str, char sep)
{
	std::vector<std::string> result;
	size_t start = 0, sep_pos = -1;

	while ((sep_pos = str.find(sep, start)))
	{
		result.push_back(str.substr(start, sep_pos - start));
		start = sep_pos + 1;
		while (str[start] == sep)
			start++;
		if (sep_pos == std::string::npos)
			break;
	}
	return (result);
}
