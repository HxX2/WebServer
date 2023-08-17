#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <iostream>
#include <vector>

namespace utils
{
	typedef std::vector<std::string> t_str_arr;

	void			trim_str(std::string &str);
	void			remove_comments(std::string &str);
	t_str_arr		split_str(std::string str, char sep);
	t_str_arr		split_str(std::string str, std::string sep);
	size_t			find_in(std::string& line, const char *chars, size_t pos, const char *error);
	size_t			not_find_in(std::string& line, const char *chars, size_t pos, const char *error);
	bool			vector_contains(t_str_arr &array, const std::string &element);
};

#endif
