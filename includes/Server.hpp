/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cipher <cipher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 21:53:21 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/22 13:28:08 by cipher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <EventHandler.hpp>
#include <Response.hpp>
#include <Logger.hpp>

class Server : public EventEmitter<Server> , public Logger
{
	private:
		struct sockaddr_in	_serverAddress;
		int					_serverSocket;
		char				_buffer[1024];
		int					_clientSocket;
		int					_opt;

		fd_set				_currentFds;
		fd_set				_writeFds;
		fd_set				_readFds;

		Response	_getres();
	public:
		Server();
		Server(int port);
		~Server();
		 
		void	Start();
		bool	Stop();
		void 	LogRequest();
		void	LogResponse();
		void 	SendResponse();
		bool 	endsWithCRLF(const char* buffer, size_t size);
};

#endif
