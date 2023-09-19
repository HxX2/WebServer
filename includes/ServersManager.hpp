/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 19:34:54 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 05:52:26 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERSMANAGER_HPP_
#define _SERVERSMANAGER_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <Server.hpp>
#include <Config.hpp>
#include <utils.hpp>

class ServersManager
{
private:
	std::vector<Server *> _servers;
	Config _config;

	fd_set _currentFds;
	fd_set _writeFds;
	fd_set _readFds;

public:
	ServersManager();
	~ServersManager();

	void addServer(Server *server);
	void loadConfig(Config &config);
	void removeServer(Server server);
	void startServers();
	void stopServers();
};

#endif
