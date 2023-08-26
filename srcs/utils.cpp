#include "utils.hpp"

void	utils::remove_comments(std::string &str)
{
	size_t s = str.find('#');
	if (s != std::string::npos)
		str.erase(s);
	size_t bracket = str.find('{');
	if (bracket != std::string::npos)
		str.erase(bracket, 1);
}

void	utils::trim_str(std::string &str)
{
	size_t s, e;

	if (str.empty()) return;
	for (s = 0; str[s] == ' ' || str[s] == '\t'; s++);
	str.erase(0, s);
	for (e = str.size() - 1; !str.empty() && (str[e] == ' ' || str[e] == '\t'); e--);
	str.erase(e + 1);
	if (!str.empty() && str.at(str.size() - 1) == ';')
		str.erase(str.size() - 1);
	if ((s = str.find_first_of(" \t")) != std::string::npos)
	{
		e = str.find_first_not_of(" \t", s);
		str = str.substr(0, s) + " " + str.substr(e);
	}
}

utils::t_str_arr	utils::split_str(std::string str, char sep)
{
	utils::t_str_arr result;
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

utils::t_str_arr	utils::split_str(std::string str, std::string sep)
{
	utils::t_str_arr result;
	size_t start = 0, sep_pos = -1;

	while ((sep_pos = str.find(sep, start)))
	{
		result.push_back(str.substr(start, sep_pos - start));
		start = sep_pos + 1;
		while (sep.find(str[start]) != std::string::npos)
			start++;
		if (sep_pos == std::string::npos)
			break;
	}
	return (result);
}

size_t	utils::find_in(std::string& line, const char *chars, size_t pos, const char *error)
{
	size_t i;

	if ((i = line.find_first_of(chars, pos)) == std::string::npos)
		throw std::invalid_argument(error);
	return (i);
}

size_t	utils::not_find_in(std::string& line, const char *chars, size_t pos, const char *error)
{
	size_t i;

	if ((i = line.find_first_not_of(chars, pos)) == std::string::npos)
		throw std::invalid_argument(error);
	return (i);
}

bool	utils::vector_contains(utils::t_str_arr &array, const std::string &element)
{
	for (size_t i = 0; i < array.size(); i++)
	{
		if (array[i] == element)
			return (true);
	}
	return (false);
}

unsigned int utils::hex_to_decimal( const std::string &hex )
{
    unsigned int dec;
    std::stringstream ss;

    ss << std::hex << hex;
    ss >> dec;

    return dec;
}
