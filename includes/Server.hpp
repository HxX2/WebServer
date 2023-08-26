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
#include <sstream>

#include <Modules.hpp>
#include <Response.hpp>

class Server : public EventEmitter<Server>, public Logger
{
private:
	struct sockaddr_in _serverAddress;
	int _serverSocket;
	char _buffer[1024];
	int _clientSocket;
	int _opt;
	std::list<int> _clients;

	fd_set _currentFds;
	fd_set _writeFds;
	fd_set _readFds;

	Response _getres();

public:
	Server();
	Server(int port, std::string address);
	~Server();

	void Start();
	bool Stop();
	void LogRequest();
	void LogResponse();
	void SendResponse(int clientSocket);
	bool endsWithCRLF(const char *buffer, size_t size);

	std::string to_string(int value) const
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
};

#endif
