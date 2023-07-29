#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <stack>
#include <vector>
#include <exception>
#include <cstdlib>
#include <arpa/inet.h>

#include "utils.hpp"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

class LocationBlock;
class ServerBlock;
class Config;

// ================= TYPEDEFS
typedef std::map<std::string, std::string>		t_directives;
typedef std::vector<LocationBlock *>			t_locations;
typedef std::vector<ServerBlock *>				t_servers;
typedef enum { SERVER, LOCATION, GLOBAL }		t_block;

struct t_directive
{
	std::string		key;
	std::string		value;

	void	parse(std::string &directive)
	{
		size_t			separator_i;

		separator_i = directive.find_first_of(" \t");
		key = directive.substr(0, separator_i);
		value = separator_i != std::string::npos
			  ? directive.substr(separator_i + 1)
			  : "";
	}

	bool	is_key_valid()
	{  
		const std::string valid_directives[] = { "host", "port", "server_name", "redirect", "allowed_methods", "index", "root", "max_body_size", "autoindex", "error_page", "cgi", "redirect" };

		for (size_t i = 0; i < 12; i++)
			if (key == valid_directives[i]) return (true);
		return (false);
	}

	bool	is_valid()
	{
		return (
			is_key_valid()
			&& key.size() != 0
			&& value.size() != 0
		);
	}
};

// ================= Classes
class LocationBlock
{
	private:
		std::string			_path;
		t_directives		_directives;

	public:
		LocationBlock(void);
		~LocationBlock(void);
		LocationBlock(const LocationBlock &block);
		LocationBlock &operator=(const LocationBlock &block);

		size_t					size(void) const;
		const std::string&		get_path(void) const;
		void					set_path(std::string &line);
		const t_directives&		get_directives(void) const;
		void					add_directive(std::string &line);
};

class ServerBlock
{
	private:
		std::string			_name;
		size_t				_port;
		std::string			_address;
		t_locations			_locations;

	public:
		ServerBlock(void);
		~ServerBlock(void);
		ServerBlock(const ServerBlock &block);
		ServerBlock &operator=(const ServerBlock &block);

		size_t				get_port() const;
		void				set_port(int port);
		const std::string&	get_name() const;
		void				set_name(const std::string &name);
		const std::string&	get_address() const;
		void				set_address(const std::string &ip);

		void				add_location(LocationBlock *new_location);
		LocationBlock		*get_location(size_t index) const;
		size_t				size(void) const;
		void				set_params(std::string &line);
		bool				is_address_valid(const std::string &address) const;
		bool				is_port_valid(const size_t &port) const;
		bool				is_match(const std::string &address, const size_t port) const;
};

struct parse_params {
	std::string				tmp_line;
	t_block					block;
	ssize_t					server_index;
	ssize_t					location_index;
	std::stack<std::string>	stack;

	parse_params(void)
	{
		block = GLOBAL;
		server_index = -1;
		location_index = -1;
	}

	void	set(t_block curr_block)
	{
		block = curr_block;
		server_index += (curr_block == SERVER);
		if (curr_block == SERVER) location_index = -1;
		location_index += (curr_block == LOCATION);
	}
};

class Config
{
	private:
		std::ifstream		_config_file;
		t_servers			_servers;

	public:
		struct t_socket {
			std::string	address;
			size_t		port;
		};

		typedef std::vector<t_socket>	t_sockets;

	public:
		Config(void);
		~Config(void);
		Config(const Config &conf);
		Config &operator=(const Config &conf);

		size_t			size() const;
		t_servers		get_servers() const;
		bool			is_server(std::string &line) const;
		bool			is_location(std::string &line) const;
		void			read(std::string filename);
		bool			is_block_head(std::string &line) const;
		void			parse(parse_params &params);
		t_sockets		get_sockets();
		t_directives	get_config(const std::string &address, const size_t port, const std::string &path);
};

std::ostream&	operator<<(std::ostream& stream, const LocationBlock& location);
std::ostream&	operator<<(std::ostream& stream, const ServerBlock& server);
std::ostream&	operator<<(std::ostream& stream, const Config& conf);

#endif
