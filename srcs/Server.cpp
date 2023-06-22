/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cipher <cipher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:09:02 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/22 13:26:37 by cipher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
		this->log("ERROR", "Failed to bind socket");
		return ;
	}

	if (listen(_serverSocket, SOMAXCONN) < 0)
	{
		this->log("ERROR", "Failed to listen");
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

		_readFds = _writeFds = _currentFds;
		memset(_buffer, 0, sizeof(_buffer));
		
		int activity = select(FD_SETSIZE, &readFds, &writeFds, NULL, NULL);
		if (activity < 0)
		{
			std::cerr << RED << "[ERROR] " << RESET <<"Failed to select" << std::endl;
			return ;
		}

		if (FD_ISSET(_serverSocket, &_readFds))
		{
			_clientSocket = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
			if (_clientSocket < 0)
			{
				std::cerr << RED << "[ERROR] " << RESET <<"Failed to accept connection" << std::endl;
				return ;
			}
			
			FD_SET(clientSocket, &readFds);
			FD_SET(clientSocket, &writeFds);

			if (fcntl(_clientSocket, F_SETFL, O_NONBLOCK) < 0)
			{
				this->log("ERROR", "Failed to set client socket flags");
				return ;
			}
			
			FD_SET(_clientSocket, &_currentFds);
		}

		if (FD_ISSET(_clientSocket, &_readFds))
		{
			readSize = recv(_clientSocket, _buffer, sizeof(_buffer), 0);

			this->emit(std::string("reading"));

			this->log("DEBUG", "readSize : " + std::to_string(readSize));
			
			if (this->endsWithCRLF(_buffer, readSize))
				this->emit(std::string("readFinished"));

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

	this->emit(std::string("response"));

	return (res);
}

void Server::LogRequest()
{
	std::cout <<  YELLOW << "[REQUEST] " << RESET << _buffer << std::endl;

}

void Server::LogResponse()
{
	std::cout << BLUE << "[RESPONSE] " << RESET << "(" << Response::getCurrentDate() << ")" << GREEN << " HTTP : " << RESET << "GET"
			  << " / " << GREEN << "200" << RESET << "\n"
			  << std::endl;
}
