/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:09:02 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/28 18:28:23 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server() : _opt(1)
{
	
}

Server::Server(int port)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
	{
		std::cerr << "Error: Failed to create socket" << std::endl;
		return ;
	}

	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(port);
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		std::cerr << "Error: Failed to bind socket" << std::endl;
		perror("bind");
		return ;
	}

	if (listen(_serverSocket, SOMAXCONN) < 0)
	{
		std::cerr << "Error: Failed to listen to connections" << std::endl;
		return ;
	}

	std::cout << "Server listening on port " << port << std::endl;
}

Server::~Server()
{
	this->Stop();
}

void Server::Start()
{
	fd_set	readFds;
	int maxSocket  = _serverSocket;
	FD_ZERO(&readFds);
	FD_SET(_serverSocket, &readFds);

	while (true)
	{
		memset(_buffer, 0, sizeof(_buffer));
		int activity = select(maxSocket + 1, &readFds, NULL, NULL, NULL);
		if (activity < 0)
		{
			std::cerr << "Error: Failed to select" << std::endl;
			return ;
		}

		if (FD_ISSET(_serverSocket, &readFds))
		{
			int clientSocket = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
			if (clientSocket < 0)
			{
				std::cerr << "Error: Failed to accept connection" << std::endl;
				return ;
			}
			
			std::cout << "New connection" << std::endl;

			FD_SET(clientSocket, &readFds);
			if (clientSocket > maxSocket)
				maxSocket = clientSocket;
			recv(clientSocket, _buffer, sizeof(_buffer), 0);
			std::cout << "Client: " << _buffer << std::endl;
		}
	}
}

bool Server::Stop()
{
	std::cout << "Server stopped" << std::endl;
	return (close(_serverSocket) == 0);
}
