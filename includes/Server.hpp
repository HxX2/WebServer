/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 21:53:21 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 04:50:17 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <list>
#include <string.h>
#include <Client.hpp>

class Client;

class Server
{
private:
	struct sockaddr_in _serverAddress;
	int _server_socket;
	int _opt;
	std::list<Client *> _clients;
	Config &_server_config;

public:
	Server();
	Server(Config &server_config, int port, std::string address);
	~Server();

	int getServerSocket() const;
	void Start(fd_set *readfds, fd_set *writefds, fd_set *currentfds);
	bool Stop();
};

#endif
