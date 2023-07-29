#include "Request.hpp"

Request::Request(void) {}

Request::Request(char *buffer)
{
	_stream << buffer;
	if (_stream.str().empty())
		throw std::invalid_argument("empty request");
	_parse_request();
	_check_request();
}

Request::~Request(void) {}

Request::Request(const Request &conf) { *this = conf; }

Request	&Request::operator=(const Request &conf)
{
	(void)conf;
	return (*this);
}

void	Request::_parse_request(void)
{
	size_t		crlf_s, crlf_e;
	std::string	tmp;

	while ((crlf_e = _stream.str().find_first_of("\r\n")) != std::string::npos)
	{
		
		std::cout << "[" << tmp.substr() << "]";
	}
}

void	Request::_check_request(void)
{}

// std::ostream&	operator<<(std::ostream& stream, const Request& request) {}
