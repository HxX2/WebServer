#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>
#include <vector>
#include <map>
#include <stack>
#include <cstdlib>
#include "utils.hpp"

class LocationBlock
{
public:
	typedef std::map<std::string, std::string> t_directives_map;

private:
	std::string _path;

public:
	std::vector<std::string> _directives;

	LocationBlock(void)
	{
	}

	~LocationBlock(void)
	{
	}

	LocationBlock(const LocationBlock &block)
	{
		(void)block;
	}

	LocationBlock &operator=(const LocationBlock &block)
	{
		(void)block;
		return (*this);
	}

	void add_directive(std::string &line);
};

class ServerBlock
{
private:
	std::string _ip;
	int _port;
	std::string _name;

public:
	std::vector<LocationBlock> _locations;

	ServerBlock(void) {}

	~ServerBlock(void) {}

	ServerBlock(const ServerBlock &block) { *this = block; }

	ServerBlock &operator=(const ServerBlock &block)
	{
		(void)block;
		return (*this);
	}

	void set_ip(const std::string &ip) { _ip = ip; }

	void set_port(int port) { _port = port; }

	void set_name(const std::string &name) { _name = name; }

	void add_location(size_t location_index, std::string &line);
};

class Config
{
public:
	typedef enum
	{
		SERVER,
		LOCATION,
		NONE
	} t_block;

private:
	std::ifstream _config_file;

public:
	std::vector<ServerBlock> _servers;

	Config(void);
	~Config(void);
	Config(const Config &conf);
	Config &operator=(const Config &conf);

	static bool is_server(std::string &line);
	static bool is_location(std::string &line);
	void parse_line(size_t &server_index, size_t &location_index, t_block &context, std::string &line, std::stack<std::string> &parse_stack);
	void read(std::string config_filename);
	void add(t_block context, size_t server_index, size_t location_index, std::string &line);
	void display(void);
};

#endif
