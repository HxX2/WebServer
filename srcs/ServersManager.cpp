#include "ServersManager.hpp"

ServersManager::ServersManager(Config &config) : _config(config)
{
	loadConfig();
}

ServersManager::~ServersManager()
{
}

void ServersManager::loadConfig()
{
	Config::t_sockets sockets;
	Config::t_sockets::iterator it;

	sockets = _config.get_sockets();
	for (it = sockets.begin(); it != sockets.end(); it++)
		this->addServer(new Server(_config, it->port, it->address));
}

void ServersManager::addServer(Server *server)
{
	if (!server->_has_failed)
		this->_servers.push_back(server);
	else
		delete server;
}

void ServersManager::startServers()
{
	std::vector<Server *>::iterator it;

	FD_ZERO(&_currentFds);
	for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
		FD_SET((*it)->getServerSocket(), &_currentFds);

	while (true)
	{
		_readFds = _writeFds = _currentFds;

		if (select(FD_SETSIZE, &_readFds, &_writeFds, NULL, NULL) < 0)
			utils::log("ERROR", "Failed to select");
		else
		{
			for (it = this->_servers.begin(); it != this->_servers.end(); it++)
				(*it)->Start(&_readFds, &_writeFds, &_currentFds);
		}
	}
}

void ServersManager::stopServers()
{
	std::vector<Server *>::iterator it;

	for (it = this->_servers.begin(); it != this->_servers.end(); it++)
		(*it)->Stop();
}
