#include "ServersManager.hpp"

ServersManager::ServersManager()
{
}

ServersManager::~ServersManager()
{
}

void ServersManager::addServer(Server server)
{
    this->_servers.push_back(server);
}

void ServersManager::removeServer(Server server)
{
    (void)server;
    // this->_servers.erase(server);
}

void ServersManager::startServers()
{
    std::vector<Server>::iterator it = this->_servers.begin();
    while (it != this->_servers.end())
    {
        it->emit(std::string("ready"));
        it++;
    }
}

void ServersManager::stopServers()
{
    std::vector<Server>::iterator it = this->_servers.begin();
    while (it != this->_servers.end())
    {
        it->emit(std::string("ready"));
        it++;
    }
}
