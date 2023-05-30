/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:20:30 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/30 01:42:35 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EVENTHANDLER_HPP_
# define _EVENTHANDLER_HPP_

#include <iostream>
#include <map>
#include <vector>
#include <functional>

class EventHandler
{
	public:
		void sayHello() { std::cout << "Hello" << std::endl; }
		void sayGoodbye() { std::cout << "Goodbye" << std::endl; }
};

typedef void (EventHandler::*Callback)();

class CallbackInvoker
{
	private:
		EventHandler	*_handler;
		Callback		_callback;
	public:
		CallbackInvoker(EventHandler *handler, Callback callback);
		void operator()();
};

class EventEmitter : public EventHandler
{
	private:
		std::map<std::string, std::vector<std::pair<EventHandler *, Callback> > > _eventCallbacks;
	public:
		void on(std::string& event, EventHandler *handler, Callback callback);
		void emit(std::string& event);
		
};


#endif