/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:09:02 by zlafou            #+#    #+#             */
/*   Updated: 2023/09/28 20:02:12 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server(Config &server_config, int port, std::string address) : _server_config(server_config)
{
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket < 0)
		utils::log("ERROR", "Failed to create socket");

	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) < 0)
		utils::log("ERROR", "Failed to set socket flags");

	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(port);
	_serverAddress.sin_addr.s_addr = inet_addr(address.c_str());

	setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));

	if (bind(_server_socket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
		utils::log("ERROR", "Failed to bind socket");

	if (listen(_server_socket, SOMAXCONN) < 0)
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

int Server::getServerSocket() const
{
	return (_server_socket);
}

void Server::Start(fd_set *readfds, fd_set *writefds, fd_set *currentfds)
{
	int client_socket;

	if (FD_ISSET(_server_socket, readfds))
	{
		client_socket = accept(_server_socket, (struct sockaddr *)NULL, NULL);

		if (client_socket < 0)
			utils::log("ERROR", "Failed to accept client");

		std::cout << YELLOW << "[REQUEST] " << RESET << std::endl;

		if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
			utils::log("ERROR", "Failed to set client socket flags");

		FD_SET(client_socket, currentfds);
		_clients.push_back(new Client(client_socket, _server_socket));
	}

	for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (FD_ISSET((*it)->_client_socket, readfds))
		{
			if (!(*it)->_is_request_ready)
				(*it)->handle_request(_server_config);
			if ((*it)->_is_request_ready)
			{
				if ((*it)->_status == "200")
					(*it)->log_reuqest();
			}
		}
		if (FD_ISSET((*it)->_client_socket, writefds))
		{
			if ((*it)->_is_request_ready)
			{
				if (!(*it)->send_body)
					(*it)->handle_response();
				(*it)->send_response();
				if ((*it)->remove_client)
				{
					(*it)->log_response();
					utils::log("INFO", "Client disconnected");
					close((*it)->_client_socket);
					FD_CLR((*it)->_client_socket, currentfds);
					(*it)->close_temp_file(true);
					delete *it;
					_clients.erase(it);
					it = _clients.begin();
				}
			}
		}
	}
}

bool Server::Stop()
{
	std::cout << "Server stopped" << std::endl;
	return (close(_server_socket) == 0);
}
