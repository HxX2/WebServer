#include "Config.hpp"

// ========== ServerBlock Class

ServerBlock::ServerBlock(void) {
	_name = "";
	_name = "127.0.0.1";
	_port = 4000;
}

ServerBlock::~ServerBlock(void) {}

ServerBlock::ServerBlock(const ServerBlock &block) { *this = block; }

ServerBlock &ServerBlock::operator=(const ServerBlock &block)
{
	_name = block._name;
	_port = block._port;
	_address = block._address;
	_locations = block._locations;
	return (*this);
}

bool	ServerBlock::is_address_valid(const std::string &address) const
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, address.c_str(), &(sa.sin_addr));
	return result;
}

bool	ServerBlock::is_port_valid(const size_t &port) const
{
	return (port > 0 && port <= 65535);
}

void ServerBlock::set_name(const std::string &name) { _name = name; }

const std::string &ServerBlock::get_name() const { return (_name); }

size_t ServerBlock::get_port() const { return (_port); }

void ServerBlock::set_port(int port) { _port = port; }

void ServerBlock::set_address(const std::string &ip) { _address = ip; }

const std::string &ServerBlock::get_address() const { return (_address); }

void ServerBlock::add_location(LocationBlock *new_location)
{
	_locations.push_back(new_location);
}

LocationBlock	*ServerBlock::get_location(size_t index) const
{
	return (_locations.at(index));
}

size_t	ServerBlock::size(void) const
{
	return (_locations.size());
}

void ServerBlock::set_params(std::string &line)
{
	t_directive directive;

	directive.parse(line);
	if (!directive.is_valid())
		throw std::invalid_argument("server in config contains invalid directive");
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
		if (!is_port_valid(std::atoi(directive.value.c_str())))
			throw std::invalid_argument("invalid port number");
		set_port(std::atoi(directive.value.c_str()));
	}
	else
	{
		for (size_t i = 0; i < _locations.size(); i++)
			_locations[i]->add_directive(line);
	}
}

bool	ServerBlock::is_match(const std::string &address, const size_t port) const
{
	return (address == this->_address && port == this->_port);
}

std::ostream &operator<<(std::ostream &stream, const ServerBlock &server)
{
	stream << "NAME = " << server.get_name() << ", IP = " << server.get_address() << ", PORT = " << server.get_port() << std::endl;
	for (size_t j = 0; j < server.size(); j++)
	{
		stream << BLUE << "\tLocation [ " << j + 1 << " ]: " << RESET << "\"" << server.get_location(j)->get_path() << "\"" << std::endl;
		stream << *(server.get_location(j));
	}
	return (stream);
}
