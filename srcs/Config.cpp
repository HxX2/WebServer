#include "Config.hpp"

Config::Config(void)
{
}

Config::~Config(void)
{
	utils::log("DEBUG", "Config destructor");
	for (size_t i = 0; i < _servers.size(); i++)
	{
		for (size_t j = 0; j < _servers[i]->size(); j++)
			delete _servers[i]->get_location(j);
		delete _servers[i];
	}
	_config_file.close();
	_servers.clear();
}

Config::Config(const Config &conf) { *this = conf; }

Config &Config::operator=(const Config &conf)
{
	_servers = conf._servers;
	return (*this);
}

size_t Config::size() const
{
	return (_servers.size());
}

std::vector<ServerBlock *> Config::get_servers() const
{
	return (_servers);
}

ServerBlock Config::get_server(size_t index) const
{
	return (*_servers[index]);
}

bool Config::is_server(std::string &line) const
{
	return (
		!line.compare(0, 6, "server") &&
		(std::string(" {\n").find(line[6]) != std::string::npos || line.size() == 6));
}

bool Config::is_location(std::string &line) const
{
	return (
		!line.compare(0, 8, "location") &&
		(std::string(" {\n").find(line[8]) != std::string::npos || line.size() == 8));
}

bool Config::is_block_head(std::string &line) const
{
	return (is_server(line) || is_location(line));
}

void Config::throw_error(parsing_params &params, std::string error, bool print_line_number = true) const
{
	std::string exception_msg = print_line_number
									? "Line " + utils::to_string(params.line_number) + ": " + error
									: error;

	throw std::invalid_argument(exception_msg);
}

void Config::read_config(std::string &filename)
{
	parsing_params params;

	_config_file.open(filename.c_str());
	if (!_config_file.is_open())
		throw std::invalid_argument("Unable to open config file '" + filename + "'");
	while (std::getline(_config_file, params.tmp_line))
	{
		utils::remove_comments(params.tmp_line);
		utils::trim_str(params.tmp_line);
		if (!params.tmp_line.empty())
			parse_config(params);
		params.line_number++;
	}
	if (params.stack.size() != 0)
		throw_error(params, "Brakets not closed properly");
	if (this->size() == 0)
		throw_error(params, "Config file can't be empty", false);
}

void Config::parse_config(parsing_params &params)
{
	if (is_block_head(params.tmp_line) || params.tmp_line != "}")
	{
		params.stack.push(params.tmp_line);
		if (is_server(params.tmp_line))
			params.set(SERVER);
		else if (is_location(params.tmp_line))
		{
			if (params.block == GLOBAL)
				throw_error(params, "Location block needs to be inside a server block");
			params.set(LOCATION);
		}
		else if (*(params.tmp_line.end() - 1) == '{')
			params.set(GLOBAL);
	}
	else if (params.tmp_line == "}")
	{
		if (_servers.size() <= (size_t)params.server_index)
			_servers.push_back(new ServerBlock);
		if (params.block == LOCATION && _servers[params.server_index]->size() <= (size_t)params.location_index)
			_servers[params.server_index]->add_location(new LocationBlock);
		while (!params.stack.empty() && !is_block_head(params.stack.top()))
		{
			if (params.block == GLOBAL)
				throw_error(params, "Invalid config block use either server or location");
			else if (params.block == SERVER)
				_servers[params.server_index]->set_params(params.stack.top(), _hosts);
			else if (params.block == LOCATION)
				_servers[params.server_index]->get_location(params.location_index)->add_directive(params.stack.top());
			params.stack.pop();
		}
		if (params.block == SERVER && _servers[params.server_index]->size() == 0)
			throw_error(params, "Server block needs to have at least 1 location");
		else if (params.block == SERVER && _servers[params.server_index]->get_port() == 0)
			throw_error(params, "Server block needs to have a port number");
		else if (params.block == SERVER && _servers[params.server_index]->get_address().empty())
			throw_error(params, "Server block needs to have an address");
		else if (params.block == LOCATION)
		{
			if (_servers[params.server_index]->get_location(params.location_index)->size() == 0)
				throw_error(params, "Location block needs to have at least 1 directive");
			_servers[params.server_index]->get_location(params.location_index)->set_path(params.stack.top());
			_servers[params.server_index]->add_path(params.stack.top());
		}
		params.stack.pop();
		params.toggle_block();
	}
}

void Config::delete_config(void)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		for (size_t j = 0; j < _servers[i]->size(); j++)
			delete _servers[i]->get_location(j);
		delete _servers[i];
	}
	_config_file.close();
}

bool compare_servers(ServerBlock *first, ServerBlock *second)
{
	return (first->get_port() == second->get_port() && first->get_address() == second->get_address());
}

std::vector<t_socket> Config::get_sockets()
{
	std::vector<t_socket> result;
	std::vector<ServerBlock *> temp = get_servers();
	std::vector<ServerBlock *>::iterator it;

	std::sort(temp.begin(), temp.end(), compare_servers);
	it = std::unique(temp.begin(), temp.end(), compare_servers);
	temp.resize(std::distance(temp.begin(), it));
	for (size_t i = 0; i < temp.size(); i++)
	{
		t_socket tmp;
		tmp.address = temp[i]->get_address();
		tmp.port = temp[i]->get_port();
		result.push_back(tmp);
	}
	return (result);
}

int compare(const std::string &request_path, const std::string &location_path)
{
	size_t i = 0;

	while (i < request_path.size() && i < location_path.size() && request_path[i] == location_path[i])
		i++;
	if (
		(request_path[i] == '\0' && location_path[i] == '\0') ||
		(request_path[i] == '/' && location_path[i] == '\0') ||
		(request_path[i - 1] == '/' && location_path[i - 1] == '/' && location_path[i] == '\0'))
		return (i);
	return (-1);
}

std::string get_directory(const std::string &path)
{
	size_t last_slash;

	if (path.empty())
		return std::string("");
	last_slash = path.find_last_of('/');
	if (
		last_slash != std::string::npos && last_slash != path.size() - 1 && path.find('.', last_slash) != std::string::npos)
		return path.substr(0, last_slash + 1);
	return std::string(path + (path[path.size() - 1] == '/' ? "" : "/"));
}

std::map<std::string, std::string> Config::get_config(const std::string &server_address, size_t server_port, const std::string &server_name, std::string &requested_path)
{
	ssize_t server_i = -1, max_similarity = -1, location_i = -1;
	utils::t_str_arr entries;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		if (_servers[i]->is_match(server_address, server_port))
		{
			if (server_i == -1)
				server_i = i;
			if (_servers[i]->get_name() == "\"\"")
			{
				server_i = i;
				continue;
			}
			entries = utils::split_str(_servers[i]->get_name(), ' ');
			if (utils::vector_contains(entries, server_name))
			{
				server_i = i;
				entries.clear();
				break;
			}
		}
	}
	for (size_t i = 0; i < _servers[server_i]->size(); i++)
	{
		ssize_t similarity = compare(get_directory(requested_path), _servers[server_i]->get_paths()[i]);
		if (similarity > max_similarity)
		{
			max_similarity = similarity;
			location_i = i;
		}
	}
	if (location_i == -1)
		return (_servers[server_i]->get_error_pages());
	else
	{
		if (_servers[server_i]->get_location(location_i)->get_path() == requested_path)
			requested_path = "/";
		else if (_servers[server_i]->get_location(location_i)->get_path() != "/")
			requested_path.erase(0, _servers[server_i]->get_location(location_i)->get_path().size());
	}
	return (_servers[server_i]->get_location(location_i)->get_directives());
}

std::ostream &operator<<(std::ostream &stream, const Config &conf)
{
	for (size_t i = 0; i < conf.size(); i++)
	{
		stream << GREEN << "==> Server [ " << i + 1 << " ]: " << RESET;
		stream << conf.get_server(i);
	}
	return stream;
}
