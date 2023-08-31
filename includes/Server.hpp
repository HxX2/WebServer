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
	int _serverSocket;
	int _opt;
	std::list<Client *> _clients;

public:
	Server();
	Server(int port, std::string address);
	~Server();

	void Start(fd_set *readfds, fd_set *writefds, fd_set *currentfds);
	int getServerSocket() const;
	bool Stop();
};

#endif
