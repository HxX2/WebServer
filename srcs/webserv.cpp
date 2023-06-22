/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cipher <cipher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:58:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/22 07:40:47 by cipher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "EventHandler.hpp"
#include "Config.hpp"

// #define PORT 8080

// int main()
// {
// 	Server server(PORT);

// 	server.on(std::string("ready"), &server, &Server::Start);

// 	server.on(std::string("request"), &server, &Server::LogRequest);

// 	server.on(std::string("response"), &server, &Server::LogResponse);

// 	server.emit(std::string("ready"));

// 	return (0);
// }

void print_error(std::string error)
{
	std::cerr << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	Config cnf;

	if (argc != 2)
		std::cerr << "Usage: ./webserv [config_file.conf]" << std::endl;
	else
	{
		try
		{
			cnf.read(argv[1]);
			cnf.display();
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return (0);
}
