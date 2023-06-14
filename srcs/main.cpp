/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:58:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/12 15:36:16 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <EventHandler.hpp>
#include <thread>

#define PORT 8080

int main()
{
	Server server(PORT);

	server.setDebug(false);

	server.on(std::string("ready"), &server, &Server::Start);

	server.on(std::string("reading"), &server, &Server::LogRequest);
	server.on(std::string("response"), &server, &Server::LogResponse);

	server.on(std::string("readFinished"), &server, &Server::SendResponse);

	server.emit(std::string("ready"));

	return (0);
}