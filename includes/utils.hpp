#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <iostream>
#include <vector>

namespace utils
{
	void trim_str(std::string &str);
	void remove_comments(std::string &str);
	std::vector<std::string> split_str(std::string str, char sep);
};

#endif
