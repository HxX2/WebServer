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
typedef std::map<std::string, std::string>	t_directives;
typedef std::vector<LocationBlock *>		t_locations;
typedef std::vector<ServerBlock *>			t_servers;
typedef enum { SERVER, LOCATION, NONE }		t_block;

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

		size_t				get_directives_count(void) const;
		void				set_path(std::string &path);
		const std::string&	get_path(void) const;
		t_directives&		get_all_directives(void);
		void				add_directive(std::string &line);
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
		const std::string&	get_name() const;
		void				set_name(const std::string &name);
		const std::string&	get_address() const;
		void				set_address(const std::string &ip);

		void			add_location(LocationBlock *new_location);
		LocationBlock	*get_location(size_t index) const;
		size_t			get_locations_count(void) const;
		void			set_params(std::string &line);
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

		t_servers		get_servers() const;
		size_t			get_servers_count() const;
		bool			is_server(std::string &line);
		bool			is_location(std::string &line);
		void			read(std::string config_filename);
		void			parse(size_t &server_index, size_t &location_index, t_block &context, std::string &line, std::stack<std::string> &parse_stack);
};

std::ostream&	operator<<(std::ostream& stream, const LocationBlock& location);
std::ostream&	operator<<(std::ostream& stream, const ServerBlock& server);
std::ostream&	operator<<(std::ostream& stream, const Config& conf);

#endif
