#include "Config.hpp"

Config::Config(void) {}

Config::~Config(void) {}

Config::Config(const Config &conf) { *this = conf; }

Config &Config::operator=(const Config &conf)
{
	_servers = conf._servers;
	return (*this);
}

t_servers Config::get_servers() const { return (_servers); }

size_t Config::size() const { return (_servers.size()); }

bool Config::is_server(std::string &line) const
{
	return (
		!line.compare(0, 6, "server")
		&& (std::string(" {\n").find(line[6]) != std::string::npos || line.size() == 6)
	);
}

bool Config::is_location(std::string &line) const
{
	return (
		!line.compare(0, 8, "location")
		&& std::string(" {\n").find(line[8]) != std::string::npos
	);
}

void Config::read(std::string filename)
{
	parse_params params;

	_config_file.open(filename.c_str());
	if (_config_file.fail())
		throw std::invalid_argument("Unable to open config file");
	else
	{
		while (std::getline(_config_file, params.tmp_line))
		{
			utils::remove_comments(params.tmp_line);
			utils::trim_str(params.tmp_line);
			if (!params.tmp_line.empty()) parse(params);
		}
		if (params.stack.size() != 0)
			throw std::invalid_argument("Brakets not closed properly");
		if (this->size() == 0)
			throw std::invalid_argument("The config file is empty");
	}
}

bool	Config::is_block_head(std::string &line) const
{
	return (is_server(line) || is_location(line));
}

void	Config::parse(parse_params &params)
{
	if (is_block_head(params.tmp_line) || params.tmp_line != "}")
	{
		params.stack.push(params.tmp_line);
		if (is_server(params.tmp_line))
			params.set(SERVER);
		else if (is_location(params.tmp_line))
		{
			if (params.block == GLOBAL)
				throw std::invalid_argument("Location block needs to be inside a server block");
			params.set(LOCATION);
		}
	}
	else if (params.tmp_line == "}")
	{
		while (!params.stack.empty() && !is_block_head(params.stack.top()))
		{
			if (_servers.size() <= (size_t)params.server_index)
				_servers.push_back(new ServerBlock);
			if (params.block == SERVER)
				_servers[params.server_index]->set_params(params.stack.top());
			else
			{
				if (_servers[params.server_index]->size() <= (size_t)params.location_index)
					_servers[params.server_index]->add_location(new LocationBlock);
				_servers[params.server_index]->get_location(params.location_index)->add_directive(params.stack.top());
			}
			params.stack.pop();
		}
		if (params.block == SERVER && _servers[params.server_index]->size() == 0)
			throw std::invalid_argument("Server block needs to have at least 1 location");
		else if (params.block == LOCATION)
		{
			_servers[params.server_index]->add_path(params.stack.top());
			_servers[params.server_index]->get_location(params.location_index)->set_path(params.stack.top());
		}
		params.stack.pop();
		params.block = (params.block == LOCATION ? SERVER : GLOBAL);
	}
}

Config::t_sockets	Config::get_sockets()
{
	t_sockets	result;
	t_socket	tmp;
	bool		is_new;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		is_new = true;
		for (size_t j = 0; j < result.size(); j++)
		{
			if (_servers[i]->is_match(result[j].address, result[j].port))
			{
				is_new = false;
				break ;
			}
		}
		if (is_new)
		{
			tmp.address = _servers[i]->get_address();
			tmp.port = _servers[i]->get_port();
			result.push_back(tmp);
		}
	}
	return (result);
}

ssize_t compare(const std::string &request_path, const std::string &location_path)
{
    ssize_t	similar_chars = 0;
	size_t	i;

    for (i = 0; i < request_path.size() && i < location_path.size(); i++)
    {
        if (request_path[i] != location_path[i])
            return (-1);
        similar_chars++;
    }
	if (
		(request_path[i] == '\0' && location_path[i] == '\0')
		|| (request_path[i] == '/' && request_path[i+1] == '\0' && location_path[i] == '\0')
		|| (request_path[i] == '\0' && location_path[i] == '/' && location_path[i+1] == '\0')
	)
    	return (similar_chars);
	return (-1);
}

std::string	get_directory(const std::string &path)
{
	size_t last_slash;
	
	if (path.empty())
		return std::string("");
	last_slash = path.find_last_of('/');
	if (
		last_slash != std::string::npos
		&& last_slash != path.size() - 1
		&& path.find('.', last_slash) != std::string::npos
	)
		return path.substr(0, last_slash+1);
	return std::string(path + (path[path.size() - 1] == '/' ? "" : "/"));
}

t_directives Config::get_config(Request &request)
{
	ssize_t				server_i = -1, max_similarity = -1, location_i = -1;
	utils::t_str_arr	entries;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		if (_servers[i]->is_match(request.getAddress(), request.getPort()))
		{
			if (server_i == -1) server_i = i;
			if (_servers[i]->get_name() == "\"\"")
			{
				server_i = i;
				continue;
			}
			entries = utils::split_str(_servers[i]->get_name(), ' ');
			if (utils::vector_contains(entries, request.getDomain()))
			{
				server_i = i;
				entries.clear();
				break;
			}
		}
	}
	for (size_t i = 0; i < _servers[server_i]->size(); i++)
	{
		ssize_t similarity = compare(get_directory(request.getPath()), _servers[server_i]->get_paths()[i]);
		if (similarity > max_similarity)
		{
			max_similarity = similarity;
			location_i = i;
		}
	}
	if (location_i == -1)
		return (std::map<std::string, std::string>());
	return (_servers[server_i]->get_location(location_i)->get_directives());
}

std::ostream &operator<<(std::ostream &stream, const Config &conf)
{
	t_servers servers = conf.get_servers();

	for (size_t i = 0; i < conf.size(); i++)
		stream << GREEN << "==> Server [ " << i + 1 << " ]: " << RESET << *(servers[i]);
	return stream;
}
