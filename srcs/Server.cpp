/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:09:02 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 05:59:59 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server() : _opt(1)
{
}

Server::Server(int port, std::string address)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		this->log("ERROR", "Failed to create socket");

	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) < 0)
		this->log("ERROR", "Failed to set socket flags");

	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(port);
	_serverAddress.sin_addr.s_addr = inet_addr(address.c_str());

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		printf("port %d host %s \n", port, address.c_str());
		this->log("ERROR", "Failed to bind socket");
	}

	if (listen(_serverSocket, SOMAXCONN) < 0)
		this->log("ERROR", "Failed to listen");

	std::cout << YELLOW << "⚡ " << RESET << "Server listening on "
			  << "\033[4;34m"
			  << "http://" << address << ":" << port << RESET << "\n"
			  << std::endl;
}

Server::~Server()
{
	this->Stop();
}

void Server::Start()
{
	FD_ZERO(&_currentFds);
	FD_SET(_serverSocket, &_currentFds);

	std::cout << _serverSocket << std::endl;
	while (true)
	{
		_readFds = _writeFds = _currentFds;
		memset(_buffer, 0, sizeof(_buffer));

		int activity = select(FD_SETSIZE, &_readFds, &_writeFds, NULL, NULL);
		if (activity < 0)
			this->log("ERROR", "Failed to select");

		if (FD_ISSET(_serverSocket, &_readFds))
		{
			_clientSocket = accept(_serverSocket, (struct sockaddr *)NULL, NULL);

			// this->log("DEBUG", std::to_string(_clientSocket) + std::string(" is accepted"));

			if (_clientSocket < 0)
				this->log("ERROR", "Failed to accept client");

			std::cout << YELLOW << "[REQUEST] " << RESET << std::endl;

			if (fcntl(_clientSocket, F_SETFL, O_NONBLOCK) < 0)
				this->log("ERROR", "Failed to set client socket flags");

			FD_SET(_clientSocket, &_currentFds);
			_clients.push_back(_clientSocket);
		}

		for (std::list<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			// this->log("DEBUG", std::to_string(*it) + std::string(" is set ") + std::to_string(FD_ISSET(*it, &_readFds)));

			if (FD_ISSET(*it, &_readFds))
			{
				ssize_t readSize = recv(*it, _buffer, sizeof(_buffer), 0);

				this->emit(std::string("reading"));

				this->log("DEBUG", "readSize : " + this->to_string(readSize));

				this->SendResponse(*it);

				_clients.erase(it);
			}
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
	Indexer indexer;

	indexer.index(std::string("/nfs/homes/zlafou/42-cursus/WebServer"));

	std::string body = indexer.getHtml();

	res.setVersion(std::string("HTTP/1.1"));
	res.setStatus(std::string("200"), std::string("OK"));

	// res.setBody(std::string("{ \"message\" : \"Welcome to Webserv\" }"));
	// res.setHeader(std::string("Content-Type"), std::string("application/json"));

	res.setBody(body);
	res.setHeader(std::string("Content-Type"), std::string("text/html"));
	res.setHeader(std::string("Content-Length"), this->to_string(body.length()));
	res.setHeader(std::string("Date"), res.getCurrentDate());
	res.setHeader(std::string("Server"), std::string("Webserv"));

	this->emit(std::string("response"));

	return (res);
}

void Server::LogRequest()
{
	std::cout << _buffer << std::endl;
}

void Server::LogResponse()
{
	std::cout << BLUE << "[RESPONSE] " << RESET << "(" << Response::getCurrentDate() << ")" << GREEN << " HTTP : " << RESET << "GET"
			  << " / " << GREEN << "200" << RESET << "\n"
			  << std::endl;
}

void Server::SendResponse(int clientSocket)
{
	if (FD_ISSET(clientSocket, &_writeFds))
	{
		Response res = this->_getres();
		res.send(clientSocket);
	}

	close(clientSocket);
	FD_CLR(clientSocket, &_currentFds);
}

bool Server::endsWithCRLF(const char *buffer, size_t size)
{
	if (size < 2)
		return (false);
	return (buffer[size - 1] == '\n' && buffer[size - 2] == '\r');
}
