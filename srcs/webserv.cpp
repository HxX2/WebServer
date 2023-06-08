/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:58:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/07 15:02:33 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <EventHandler.hpp>
#include <thread>

#define PORT 8080

int main()
{
	Server server(PORT);

	server.on(std::string("ready"), &server, &Server::Start);

	server.on(std::string("request"), &server, &Server::LogRequest);

	server.on(std::string("response"), &server, &Server::LogResponse);

	server.emit(std::string("ready"));

	return (0);
}