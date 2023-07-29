#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <iostream>
#include <vector>

namespace utils
{
	void trim_str(std::string &str);
	void remove_comments(std::string &str);
	std::vector<std::string> split_str(std::string str, char sep);
	std::vector<std::string> split_str(std::string str, std::string sep);
	size_t	find_in(std::string& line, const char *chars, size_t pos, const char *error);
	size_t	not_find_in(std::string& line, const char *chars, size_t pos, const char *error);
};

#endif
