#ifndef __REQUEST_HPP_
# define __REQUEST_HPP_

# include <iostream>
# include <sstream>
# include <string>
# include <map>

# include "Config.hpp"

class Config;

class Request
{
	public:
		typedef std::map<std::string, std::string>	t_headers;
		typedef t_headers::const_iterator			t_headers_index;
		typedef std::map<std::string, std::string>	t_config;
		typedef t_headers::const_iterator			t_config_index;

	private:
		size_t				_socket_fd;
		std::stringstream	_stream;
		t_config			_config;
		std::string			_method;
		std::string			_path;
		std::string			_version;
		std::string			_address;
		std::string			_domain;
		size_t				_port;
		t_headers			_headers;
		std::string			_body;

	public:
		Request(void);
		Request(size_t socket_fd, char *buffer);
		~Request(void);
		Request(const Request &req);
		Request &operator=(const Request &req);

		const t_config		&getConfig() const { return (_config); }
		const std::string	&getMethod() const { return (_method); }
		const std::string	&getPath() const { return (_path); }
		const std::string	&getVersion() const { return (_version); }
		const std::string	&getAddress() const { return (_address); }
		const std::string	&getDomain() const { return (_domain); }
		size_t				getPort() const { return (_port); }
		const t_headers		&getHeaders() const { return (_headers); }
		const std::string	&getBody() const { return (_body); }

		void				parseRequest(Config &server_config);

	private:
		bool				_readline(std::string &line, bool include_CRLF = false);
		void				_setRequestLine(std::string &line);
		void				_setHeader(std::string &line);
		void				_checkRequest(void);
};

std::ostream&	operator<<(std::ostream& stream, const Request& request);

#endif
