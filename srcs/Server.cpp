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
		utils::log("ERROR", "Failed to create socket");

	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) < 0)
		utils::log("ERROR", "Failed to set socket flags");

	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(port);
	_serverAddress.sin_addr.s_addr = inet_addr(address.c_str());

	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		printf("port %d host %s \n", port, address.c_str());
		utils::log("ERROR", "Failed to bind socket");
	}

	if (listen(_serverSocket, SOMAXCONN) < 0)
		utils::log("ERROR", "Failed to listen");

	std::cout << YELLOW << "⚡ " << RESET << "Server listening on "
			  << "\033[4;34m"
			  << "http://" << address << ":" << port << RESET << "\n"
			  << std::endl;
}

Server::~Server()
{
	this->Stop();
}

void Server::Start(fd_set *readfds, fd_set *writefds, fd_set *currentfds)
{
	int client_socket;
	(void)writefds;

	if (FD_ISSET(_serverSocket, readfds))
	{
		client_socket = accept(_serverSocket, (struct sockaddr *)NULL, NULL);

		// utils::log("DEBUG", std::to_string(client_socket) + std::string(" is accepted"));

		std::cout << _serverSocket << std::endl;
		std::cout << YELLOW << "[REQUEST] " << RESET << std::endl;

		if (client_socket < 0)
			utils::log("ERROR", "Failed to accept client");

		if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
			utils::log("ERROR", "Failed to set client socket flags");

		FD_SET(client_socket, currentfds);
		_clients.push_back(new Client(client_socket));
	}

	for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		// utils::log("DEBUG", this->to_string(*it) + std::string(" is set ") + this->to_string(FD_ISSET(*it, readfds)));
		char buffer[2048];

		if (FD_ISSET((*it)->_client_socket, readfds))
		{
			std::cout << "reading" << std::endl;
			size_t i = recv((*it)->_client_socket, buffer, sizeof(buffer), 0);
			buffer[i] = '\0';

			std::cout << buffer << std::endl;
		}
		sleep(1);
		if (FD_ISSET((*it)->_client_socket, writefds))
		{
			(*it)->indexer_response("/root/LAB/WebServer");
			(*it)->send_response();
			close((*it)->_client_socket);
			FD_CLR((*it)->_client_socket, currentfds);
			delete *it;
			_clients.erase(it);
			it = _clients.begin();
		}
	}
}

bool Server::Stop()
{
	std::cout << "Server stopped" << std::endl;
	return (close(_serverSocket) == 0);
}

int Server::getServerSocket() const
{
	return (_serverSocket);
}