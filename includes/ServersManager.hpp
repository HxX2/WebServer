/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cipher <cipher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 19:34:54 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/22 13:26:11 by cipher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERSMANAGER_HPP_
#define _SERVERSMANAGER_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <Server.hpp>

class ServersManager
{
private:
    std::vector<Server> _servers;

public:
    ServersManager();
    ~ServersManager();

    void addServer(Server server);
    void removeServer(Server server);
    void startServers();
    void stopServers();
};

#endif
