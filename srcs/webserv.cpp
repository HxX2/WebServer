/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:58:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/30 01:45:40 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <EventHandler.hpp>
#include <thread>

#define PORT 8080

int main()
{
	// Server server(PORT);
	// server.Start();

	EventEmitter emitter;
	EventHandler handler;
	
	std::string hello("hello");
	std::string goodbye("goodbye");

	emitter.on(hello, &handler, &EventHandler::sayHello);
	emitter.on(goodbye, &handler, &EventHandler::sayGoodbye);

	emitter.emit(hello);

	std::this_thread::sleep_for(std::chrono::seconds(3));
			
	emitter.emit(goodbye);
		
	return (0);
}