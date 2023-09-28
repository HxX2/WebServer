#include "Config.hpp"

// ========== ServerBlock Class

ServerBlock::ServerBlock(void)
{
	_address = "";
	_name = "\"\"";
	_port = 0;
}

ServerBlock::~ServerBlock(void)
{
	_paths.clear();
	_locations.clear();
	_error_pages.clear();
}

ServerBlock::ServerBlock(const ServerBlock &block) { *this = block; }

ServerBlock &ServerBlock::operator=(const ServerBlock &block)
{
	_port = block._port;
	_address = block._address;
	_name = block._name;
	_locations = block._locations;
	_paths = block._paths;
	_error_pages = block._error_pages;
	return (*this);
}

bool ServerBlock::is_address_valid(const std::string &address) const
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, address.c_str(), &(sa.sin_addr));
	return result;
}

bool ServerBlock::is_port_valid(const size_t &port) const
{
	return (port > 0 && port <= 65535);
}

void ServerBlock::set_name(const std::string &name) { _name = name; }

const std::string &ServerBlock::get_name() const { return (_name); }

void ServerBlock::set_port(int port) { _port = port; }

size_t ServerBlock::get_port() const { return (_port); }

void ServerBlock::set_address(const std::string &ip) { _address = ip; }

const std::string &ServerBlock::get_address() const { return (_address); }

void ServerBlock::add_location(LocationBlock *new_location)
{
	_locations.push_back(new_location);
}

LocationBlock *ServerBlock::get_location(size_t index) const
{
	return (_locations.at(index));
}

size_t ServerBlock::size(void) const
{
	return (_locations.size());
}

void ServerBlock::set_params(std::string &line, std::vector<std::string> &hosts)
{
	t_directive directive;
	size_t separator_index;
	(void)hosts;

	directive.parse(line);
	if (!directive.is_valid())
		throw std::invalid_argument("server block contains invalid directive");
	if (directive.key == "server_name")
		set_name(directive.value);
	else if (directive.key == "host")
	{
		if (!is_address_valid(directive.value))
			throw std::invalid_argument("invalid ip address");
		set_address(directive.value);
	}
	else if (directive.key == "port")
	{
		int port = atoi(directive.value.c_str());
		if ((port == 0 && directive.value != "0") || !is_port_valid(port))
			throw std::invalid_argument("invalid port number");
		set_port(port);
	}
	else
	{
		// TODO: add this to t_directive as parse special
		if (directive.key == "error_page")
		{
			separator_index = directive.value.find(":");
			if (separator_index != std::string::npos)
			{
				directive.key.append("_" + directive.value.substr(0, separator_index));
				directive.value.erase(0, separator_index + 1);
			}
			if (!_error_pages.count(directive.key))
				_error_pages[directive.key] = directive.value;
		}
		for (size_t i = 0; i < _locations.size(); i++)
			_locations[i]->add_directive(line);
	}
}

bool ServerBlock::is_match(const std::string &address, const size_t port) const
{
	return (address == this->_address && port == this->_port);
}

const utils::t_str_arr &ServerBlock::get_paths(void) const
{
	return (_paths);
}

void ServerBlock::add_path(std::string &line)
{
	size_t path_start, path_end;

	path_start = line.find_first_of(" \t");
	path_end = line.find_last_of(" \t");
	_paths.push_back(line.substr(path_start + 1, path_end - path_start - 1));
}

const std::map<std::string, std::string> &ServerBlock::get_error_pages() const
{
	return (_error_pages);
}

std::ostream &operator<<(std::ostream &stream, const ServerBlock &server)
{
	stream << "NAME = " << server.get_name() << ", IP = " << server.get_address() << ", PORT = " << server.get_port() << "\n";
	stream << BLUE << "\tPATHS: " << RESET;
	for (size_t i = 0; i < server.get_paths().size(); i++)
		stream << (i == 0 ? "" : ", ") << server.get_paths()[i];
	stream << "\n";
	stream << BLUE << "\tERROR_PAGES: " << RESET << (server.get_error_pages().size() == 0 ? "No pages" : "") << "\n";
	for (std::map<std::string, std::string>::const_iterator i = server.get_error_pages().begin(); i != server.get_error_pages().end(); i++)
		stream << YELLOW << "\t\tPage [" << i->first << "]: " << RESET << i->second << "\n";
	for (size_t j = 0; j < server.size(); j++)
		stream << *(server.get_location(j));
	return (stream);
}
