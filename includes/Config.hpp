#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>
#include <cstdlib>

#include "utils.hpp"

class LocationBlock;
class ServerBLock;
class Config;

// ================= TYPEDEFS
typedef std::map<std::string, std::string>	t_directives;
typedef std::vector<LocationBlock *>		t_locations;
typedef std::vector<ServerBlock *>			t_servers;
typedef enum { SERVER, LOCATION, NONE }		t_block;

// ================= Classes
class LocationBlock
{
	private:
		std::string		_path;
		t_directives	_directives;

	public:
		LocationBlock(void);
		~LocationBlock(void);
		LocationBlock(const LocationBlock &block);
		LocationBlock &operator=(const LocationBlock &block);

		void			print_directives(void);
		size_t			get_directives_count(void);
		void			add_directive(std::string &line);
		std::string		get_directive(std::string &name);
};

class ServerBlock
{
	private:
		size_t				_port;
		std::string			_name;
		std::string			_address;
		t_locations			_locations;

	public:
		ServerBlock(void);
		~ServerBlock(void);
		ServerBlock(const ServerBlock &block);
		ServerBlock &operator=(const ServerBlock &block);

		size_t				get_port() const;
		void				set_port(int port);
		const std::string	&get_name() const;
		void				set_name(const std::string &name);
		const std::string	&get_address() const;
		void				set_address(const std::string &ip);

		// void add_location(size_t location_index, std::string &line);
		void			add_location(LocationBlock *new_location);
		LocationBlock	*get_location(size_t index) const;
		size_t			get_locations_count(void) const;
};

class Config
{
	private:
		std::ifstream		_config_file;
		t_servers			_servers;

	public:

		Config(void);
		~Config(void);
		Config(const Config &conf);
		Config &operator=(const Config &conf);
		
		const t_servers		&get_servers() const;
		size_t				get_servers_count() const;

		bool				is_server(std::string &line);
		bool				is_location(std::string &line);
		void				read(std::string config_filename);
		void				parse(size_t &server_index, size_t &location_index, t_block &context, std::string &line, std::stack<std::string> &parse_stack);
		void				display(void);
};

#endif
