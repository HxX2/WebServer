#include "Config.hpp"

const char *boolArr[2] = {"False", "True"};

Config::Config(void) {}

Config::~Config(void) {}

Config::Config(const Config &conf)
{
	*this = conf;
}

Config &Config::operator=(const Config &conf)
{
	(void)conf;
	return (*this);
}

bool Config::is_server(std::string &line)
{
	return (
		!line.compare(0, 6, "server") && (line[6] == ' ' || line[6] == '{' || line[6] == '\n'));
}

bool Config::is_location(std::string &line)
{
	return (
		!line.compare(0, 8, "location") && (line[8] == ' ' || line[8] == '{' || line[8] == '\n'));
}

void Config::parse_line(size_t &server_index, size_t &location_index, t_block &context, std::string &line, std::stack<std::string> &parse_stack)
{
	if (Config::is_server(line) || Config::is_location(line) || line != "}")
	{
		parse_stack.push(line);
		if (is_location(line))
			context = LOCATION;
		if (is_server(line))
			context = SERVER;
		server_index += is_server(line);
		if (Config::is_server(line))
			location_index = -1;
		location_index += is_location(line);
	}
	else if (line == "}")
	{
		while (
			!parse_stack.empty() && !is_server(parse_stack.top()) && !is_location(parse_stack.top()) && parse_stack.top() != "{")
		{
			std::cout << "Context: " << context << " - " << (context == SERVER ? "SERVER" : "LOCATION")
					  << ", Server Index: " << server_index
					  << ", Location Index: " << location_index << std::endl
					  << parse_stack.top() << std::endl
					  << std::endl;

			add(context, server_index, location_index, parse_stack.top());

			parse_stack.pop();
		}
		parse_stack.pop();
		if (context == LOCATION)
			context = SERVER;
	}
}

void Config::read(std::string config_filename)
{
	size_t server_index = -1;
	size_t location_index = -1;
	std::string tmp_line;
	std::stack<std::string> parse_stack;
	t_block context = NONE;

	_config_file.open(config_filename.c_str());
	if (_config_file.fail())
		throw std::invalid_argument("Error: Unable to open config file");
	else
	{
		while (std::getline(_config_file, tmp_line))
		{
			utils::trim_str(tmp_line);
			utils::remove_comments(tmp_line);
			if (!tmp_line.empty())
				parse_line(server_index, location_index, context, tmp_line, parse_stack);
		}
	}
}

void ServerBlock::add_location(size_t location_index, std::string &line)
{
	if (_locations.size() <= location_index)
	{
		LocationBlock *tmp_location = new LocationBlock();
		_locations.push_back(*tmp_location);
	}
	_locations[location_index]._directives.push_back(line);
}

void Config::add(t_block context, size_t server_index, size_t location_index, std::string &line)
{
	// if (_servers.size() < server_index + 1)
	if (_servers.size() <= server_index)
	{
		ServerBlock *tmp_server = new ServerBlock();
		_servers.push_back(*tmp_server);
	}
	if (context == LOCATION)
		_servers[server_index].add_location(location_index, line);
	// std::cout << "\tServers count: " << _servers.size()
	// 		  << ", Locations Count: " << _servers[server_index]._locations.size() << std::endl
	// 		  << std::endl;
	// else
	// {
	// 	_servers[server_index].set_ip(std::string("0.0.0.0"));
	// 	_servers[server_index].set_port(8000);
	// 	_servers[server_index].set_name(std::string("my.server.com"));
	// }
}

void Config::display(void)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		for (size_t j = 0; j < _servers[i]._locations.size(); j++)
		{
			for (size_t k = 0; k < _servers[i]._locations[j]._directives.size(); k++)
			{
				std::cout << "Server [ " << i + 1 << " ]"
						  << ", Location [ " << j + 1 << " ]"
						  << ", Directive [ " << k + 1 << " ]"
						  << " => " << _servers[i]._locations[j]._directives[k] << std::endl;
			}
		}
	}
}
