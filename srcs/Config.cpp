#include "Config.hpp"

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
				if (_servers[server_index]->_locations.size() <= location_index)
				{
					LocationBlock *location = new LocationBlock();
					_servers[server_index]->_locations.push_back(location);
				}
				std::string line = parse_stack.top();
				size_t directive_name_end = line.find_first_of(" \t");
				_servers[server_index]->_locations[location_index]->_directives[line.substr(0, directive_name_end)] = line.substr(line.find_first_not_of(" \t", directive_name_end));
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
		std::cout << "Server [ " << i + 1 << " ] : NAME = " << _servers[i]->get_name() << ", IP = " << _servers[i]->get_ip() << ", PORT = " << _servers[i]->get_port() << std::endl;
		for (size_t j = 0; j < _servers[i]->_locations.size(); j++)
		{
			std::cout << "\tLocation [ " << j + 1 << " ],      " << _servers[i]->_locations[j]->_directives.size() << std::endl;
			LocationBlock::t_directives::iterator it = _servers[i]->_locations[j]->_directives.begin();
			while (it != _servers[i]->_locations[j]->_directives.end())
			{
				std::cout << "\t\tDirective [ " << it->first << " ]"
						  << " => " << it->second << std::endl;
				it++;
			}
		}
	}
}
