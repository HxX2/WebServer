/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 21:53:21 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/06 16:22:01 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <EventHandler.hpp>
#include <Response.hpp>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"


class Server : public EventEmitter<Server>
{
	private:
		struct sockaddr_in	_serverAddress;
		int					_serverSocket;
		char				_buffer[1024];
		int					_opt;

		Response	_getres();
	public:
		Server();
		Server(int port);
		~Server();
		 
		void	Start();
		bool	Stop();
		void	Logger(std::string type);
};

#endif