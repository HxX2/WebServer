#include "Request.hpp"

Request::Request(void) {}

Request::~Request(void) {}

Request::Request(const Request &req)
{
	*this = req;
}

Request &Request::operator=(const Request &req)
{
	(void)req;
	return (*this);
}

void Request::parse(std::string req_buffer)
{
}
