#ifndef _EVENTHANDLER_HPP_
#define _EVENTHANDLER_HPP_

#include <iostream>
#include <map>
#include <vector>
#include <functional>

class Request
{
private:
public:
	Request(void);
	~Request(void);
	Request(const Request &conf);
	Request &operator=(const Request &conf);

	void parse(std::string req_buffer);
};

#endif
