#ifndef __REQUEST_HPP_
#define __REQUEST_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <map>

class Request
{
	public:
		enum method_type { GET, POST, DELETE, OTHER };
		typedef std::map<std::string, std::string> string_map;

	private:
		std::stringstream	_stream;
		string_map			_config;
		std::string			_path;
		method_type			_method;
		std::string			_host;
		string_map			_headers;
		string_map			_body;

	public:
		Request(void);
		Request(char *buffer);
		~Request(void);
		Request(const Request &conf);
		Request &operator=(const Request &conf);

	private:
		void	_parse_request(void);
		void	_check_request(void);
};

std::ostream&	operator<<(std::ostream& stream, const Request& request);

#endif
