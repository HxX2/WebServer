/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:58:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/30 15:46:39 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <EventHandler.hpp>
#include <thread>

#define PORT 8080

int main()
{
	Server server(PORT);

	server.on(std::string("goodbye"), &server, &Server::sayGoodbye);

	server.on(std::string("ready"), &server, &Server::Start);

	std::this_thread::sleep_for(std::chrono::seconds(3));
			
	server.emit(std::string("ready"));
		
	server.emit(std::string("goodbye"));

	return (0);
}