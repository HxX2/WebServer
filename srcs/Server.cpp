/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:09:02 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/07 07:40:44 by zlafou           ###   ########.fr       */
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
		std::cerr << RED << "[ERROR] " << RESET <<"Failed to create socket" << std::endl;
		return ;
	}

	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(port);
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		std::cerr << RED << "[ERROR] " << RESET <<"Failed to bind socket" << std::endl;
		return ;
	}

	if (listen(_serverSocket, SOMAXCONN) < 0)
	{
		std::cerr << RED << "[ERROR] " << RESET <<"Failed to listen to connections" << std::endl;
		return ;
	}

	std::cout << YELLOW <<"⚡ " << RESET << "Server listening on "  << "\033[4;34m" << "http://localhost:" << port  << RESET << "\n" << std::endl;
}

Server::~Server()
{
	this->Stop();
}

void Server::Start()
{
	fd_set	currentFds;
	fd_set	readFds;
	fd_set	writeFds;

	FD_ZERO(&currentFds);
	FD_SET(_serverSocket, &currentFds);
	
	while (true)
	{

		readFds = writeFds = currentFds;
		memset(_buffer, 0, sizeof(_buffer));
		
		int activity = select(FD_SETSIZE, &readFds, &writeFds, NULL, NULL);
		if (activity < 0)
		{
			std::cerr << RED << "[ERROR] " << RESET <<"Failed to select" << std::endl;
			return ;
		}

		if (FD_ISSET(_serverSocket, &readFds))
		{
			int clientSocket = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
			if (clientSocket < 0)
			{
				std::cerr << RED << "[ERROR] " << RESET <<"Failed to accept connection" << std::endl;
				return ;
			}
			
			FD_SET(clientSocket, &readFds);
			FD_SET(clientSocket, &writeFds);

			recv(clientSocket, _buffer, sizeof(_buffer), 0);
			std::cout <<  YELLOW << "[REQUEST] " << RESET << _buffer << std::endl;

			if (FD_ISSET(clientSocket, &writeFds))
			{
				Response res = this->_getres();
				res.send(clientSocket);
			}

			close(clientSocket);
			FD_CLR(clientSocket, &currentFds);
		}
	}
}

bool Server::Stop()
{
	std::cout << "Server stopped" << std::endl;
	return (close(_serverSocket) == 0);
}


Response Server::_getres()
{
	Response res = Response();

	res.setVersion(std::string("HTTP/1.1"));
	res.setStatus(std::string("200"), std::string("OK"));
	res.setBody(std::string("{ \"message\" : \"Welcome to Webserv\" }"));
	res.setHeader(std::string("Content-Type"), std::string("application/json"));
	res.setHeader(std::string("Content-Length"), std::string("36"));	
	res.setHeader(std::string("Date"), res.getCurrentDate());
	res.setHeader(std::string("Server"), std::string("Webserv"));

	std::cout <<  BLUE << "[RESPONSE] " << RESET << "(" << res.getCurrentDate() << ")" << GREEN << " HTTP : " << RESET << "GET" << " / "  << GREEN << "200" << RESET << "\n" << std::endl;
	
	return (res);
}