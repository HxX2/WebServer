#include "Request.hpp"

Request::Request(void) {}

Request::~Request(void) {}

Request::Request(const Request &req)
{
	*this = req;
}

Request &Request::operator=(const Request &req)
{
	_socket_fd = req._socket_fd;
	return (*this);
}

void Request::parse(std::string req_buffer)
{
	(void)req_buffer;
}
