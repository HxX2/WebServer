#include "Config.hpp"

// ========== LocationBlock Class

LocationBlock::LocationBlock(void) {}

LocationBlock::~LocationBlock(void) {}

LocationBlock::LocationBlock(const LocationBlock &block) { *this = block; }

LocationBlock &LocationBlock::operator=(const LocationBlock &block)
{
	_directives = block._directives;
	return (*this);
}

void LocationBlock::print_directives(void)
{
	t_directives::iterator it = _directives.begin();
	while (it != _directives.end())
	{
		std::cout << "\t\tDirective [ " << it->first << " ]"
				  << " => " << it->second << std::endl;
		it++;
	}
}

void LocationBlock::add_directive(std::string &line)
{
	size_t name_end;
	std::string name, value;

	name_end = line.find_first_of(" \t");
	name = line.substr(0, name_end);
	value = line.substr(line.find_first_not_of(" \t", name_end));
	_directives[name] = value;
}

std::string LocationBlock::get_directive(std::string &name)
{
	return (_directives[name]);
}

size_t LocationBlock::get_directives_count(void)
{
	return (_directives.size());
}

// ========== ServerBlock Class

ServerBlock::ServerBlock(void) {}

ServerBlock::~ServerBlock(void) {}

ServerBlock::ServerBlock(const ServerBlock &block) { *this = block; }

ServerBlock	&ServerBlock::operator=(const ServerBlock &block)
{
	(void)block;
	return (*this);
}

size_t	ServerBlock::get_port() const { return (_port); }
void	ServerBlock::set_port(int port) { _port = port; }

void	ServerBlock::set_name(const std::string &name) { _name = name; }
const std::string	&ServerBlock::get_name() const { return (_name); }

void	ServerBlock::set_address(const std::string &ip) { _address = ip; }
const std::string	&ServerBlock::get_address() const { return (_address); }

void	ServerBlock::add_location(LocationBlock *new_location)
{
	_locations.push_back(new_location);
}

LocationBlock	*ServerBlock::get_location(size_t index) const
{
	return (_locations.at(index));
}

size_t	ServerBlock::get_locations_count(void) const
{
	return (_locations.size());
}

// ========== Config Class

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

const t_servers &Config::get_servers() const
{
	return (_servers);
}

size_t Config::get_servers_count() const
{
	return (_servers.size());
}

bool Config::is_server(std::string &line)
{
	return (!line.compare(0, 6, "server") && std::string(" {\n").find(line[6]) != std::string::npos);
}

bool Config::is_location(std::string &line)
{
	return (!line.compare(0, 8, "location") && std::string(" {\n").find(line[8]) != std::string::npos);
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
			utils::remove_comments(tmp_line);
			utils::trim_str(tmp_line);
			if (!tmp_line.empty())
				parse(server_index, location_index, context, tmp_line, parse_stack);
		}
	}
}

void Config::parse(size_t &server_index, size_t &location_index, t_block &context, std::string &line, std::stack<std::string> &parse_stack)
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
			if (_servers.size() <= server_index)
			{
				ServerBlock *server = new ServerBlock();
				_servers.push_back(server);
			}
			if (context == LOCATION)
			{
				if (_servers[server_index]->get_locations_count() <= location_index)
				{
					LocationBlock *location = new LocationBlock();
					_servers[server_index]->add_location(location);
				}
				_servers[server_index]->get_location(location_index)->add_directive(parse_stack.top());
			}
			parse_stack.pop();
		}
		parse_stack.pop();
		context = (context == LOCATION ? SERVER : LOCATION);
	}
}

void Config::display(void)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Server [ " << i + 1 << " ] : NAME = " << _servers[i]->get_name() << ", IP = " << _servers[i]->get_address() << ", PORT = " << _servers[i]->get_port() << std::endl;
		for (size_t j = 0; j < _servers[i]->get_locations_count(); j++)
		{
			std::cout << "\tLocation [ " << j + 1 << " ]" << std::endl;
			_servers[i]->get_location(j)->print_directives();
		}
	}
}
