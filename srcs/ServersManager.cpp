/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 19:38:44 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/11 19:49:30 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServersManager.hpp>

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
    this->_servers.erase(server);
}

void ServersManager::startServers()
{
    for (std::vector<Server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
        it->emit(std::string("ready")
}

void ServersManager::stopServers()
{
    
}
