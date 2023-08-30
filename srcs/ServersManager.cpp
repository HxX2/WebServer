#include "ServersManager.hpp"

ServersManager::ServersManager()
{
}

ServersManager::~ServersManager()
{
}

void ServersManager::addServer(Server *server)
{
	this->_servers.push_back(server);
}

void ServersManager::removeServer(Server server)
{
	(void)server;
}

void ServersManager::startServers()
{
	std::vector<Server *>::iterator it;

	FD_ZERO(&_currentFds);
	for (it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		FD_SET((*it)->getServerSocket(), &_currentFds);
	}

	while (true)
	{
		_readFds = _writeFds = _currentFds;

		int activity = select(FD_SETSIZE, &_readFds, &_writeFds, NULL, NULL);
		if (activity < 0)
			utils::log("ERROR", "Failed to select");
		it = this->_servers.begin();
		while (it != this->_servers.end())
		{
			(*it)->Start(&_readFds, &_writeFds, &_currentFds);
			it++;
		}
	}
}

void ServersManager::stopServers()
{
	std::vector<Server *>::iterator it = this->_servers.begin();
	while (it != this->_servers.end())
	{
		(*it)->Stop();
		it++;
	}
}

void ServersManager::loadConfig(Config &config)
{
	Config::t_sockets sockets;
	Config::t_sockets::iterator it;

	this->_config = config;
	sockets = _config.get_sockets();
	it = sockets.begin();
	while (it != sockets.end())
	{
		this->addServer(new Server(it->port, it->address));
		it++;
	}
}
