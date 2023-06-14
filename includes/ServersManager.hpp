/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 19:34:54 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/11 19:38:04 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERSMANAGER_HPP_
# define _SERVERSMANAGER_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <Server.hpp>

class ServersManager
{
    private:
        <std::vector>Server _servers;
    public:
        ServersManager();
        ~ServersManager();
        
        void addServer(Server server);
        void removeServer(Server server);
        void startServers();
        void stopServers();
};

#endif