#include "utils.hpp"

void utils::remove_comments(std::string &str)
{
	size_t s = str.find('#');
	if (s != std::string::npos)
		str.erase(s);
	size_t bracket = str.find('{');
	if (bracket != std::string::npos)
		str.erase(bracket, 1);
}

void utils::trim_str(std::string &str)
{
	size_t s, e;

	if (str.empty())
		return;
	for (s = 0; str[s] == ' ' || str[s] == '\t'; s++)
		;
	str.erase(0, s);
	for (e = str.size() - 1; !str.empty() && (str[e] == ' ' || str[e] == '\t'); e--)
		;
	str.erase(e + 1);
	if (!str.empty() && str.at(str.size() - 1) == ';')
		str.erase(str.size() - 1);
	if ((s = str.find_first_of(" \t")) != std::string::npos)
	{
		e = str.find_first_not_of(" \t", s);
		str = str.substr(0, s) + " " + str.substr(e);
	}
}

utils::t_str_arr utils::split_str(std::string str, char sep)
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

utils::t_str_arr utils::split_str(std::string str, std::string sep)
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

size_t utils::find_in(std::string &line, const char *chars, size_t pos, const char *error)
{
	size_t i;

	if ((i = line.find_first_of(chars, pos)) == std::string::npos)
		throw std::invalid_argument(error);
	return (i);
}

size_t utils::not_find_in(std::string &line, const char *chars, size_t pos, const char *error)
{
	size_t i;

	if ((i = line.find_first_not_of(chars, pos)) == std::string::npos)
		throw std::invalid_argument(error);
	return (i);
}

bool utils::vector_contains(utils::t_str_arr &array, const std::string &element)
{
	for (size_t i = 0; i < array.size(); i++)
	{
		if (array[i] == element)
			return (true);
	}
	return (false);
}

std::string utils::to_string(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string utils::http_msg(std::string status)
{
	std::map<std::string, std::string> statusMessages;

	statusMessages["100"] = "Continue";
	statusMessages["101"] = "Switching Protocols";
	statusMessages["200"] = "OK";
	statusMessages["201"] = "Created";
	statusMessages["202"] = "Accepted";
	statusMessages["203"] = "Non-Authoritative Information";
	statusMessages["204"] = "No Content";
	statusMessages["205"] = "Reset Content";
	statusMessages["206"] = "Partial Content";
	statusMessages["300"] = "Multiple Choices";
	statusMessages["301"] = "Moved Permanently";
	statusMessages["302"] = "Found";
	statusMessages["303"] = "See Other";
	statusMessages["304"] = "Not Modified";
	statusMessages["307"] = "Temporary Redirect";
	statusMessages["308"] = "Permanent Redirect";
	statusMessages["400"] = "Bad Request";
	statusMessages["403"] = "Forbidden";
	statusMessages["404"] = "Not Found";
	statusMessages["405"] = "Method Not Allowed";
	statusMessages["409"] = "Conflict";
	statusMessages["411"] = "Length Required";
	statusMessages["413"] = "Payload Too Large";
	statusMessages["500"] = "Internal Server Error";
	statusMessages["505"] = "HTTP Version Not Supported";

	return (statusMessages[status]);
}

std::string utils::http_date()
{
	std::time_t t = std::time(0);
	std::tm *now = std::gmtime(&t);

	const char *format = "%a, %d %b %Y %H:%M:%S GMT";
	char buffer[64];

	std::strftime(buffer, sizeof(buffer), format, now);

	return (std::string(buffer));
}

void utils::log(std::string type, std::string msg)
{
	if (type == "DEBUG")
		std::cout << GREEN << "[" << type << "] " << RESET << msg << std::endl;
	else if (__DEBUG && type == "ERROR")
	{
		std::cerr << RED << "[" << type << "] " << RESET << msg << std::endl;
		if (__DEBUG)
			perror("msg");
		exit(1);
	}
	else if (type == "WARNING")
		std::cerr << YELLOW << "[" << type << "] " << RESET << msg << std::endl;
	else if (type == "INFO")
		std::cout << MAGENTA << "[" << type << "] " << RESET << msg << std::endl;
}