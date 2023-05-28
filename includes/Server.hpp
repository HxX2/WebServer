/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 21:53:21 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/28 17:59:02 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server
{
	private:
		struct sockaddr_in	_serverAddress;
		int					_serverSocket;
		char				_buffer[1024];
		int					_opt;
	public:
		Server();
		Server(int port);
		~Server();
		 
		void	Start();
		bool	Stop();
};

#endif