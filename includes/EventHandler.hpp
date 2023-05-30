/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:20:30 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/30 15:57:31 by zlafou           ###   ########.fr       */
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

template <typename HandlerType>
class CallbackInvoker
{
	typedef void (HandlerType::*Callback)();

	private:
		HandlerType		*_handler;
		Callback		_callback;
	public:
		CallbackInvoker(HandlerType *handler, Callback callback) : _handler(handler), _callback(callback) {}
		void operator()() { (_handler->*_callback)(); }
};

template <typename HandlerType>
class EventEmitter : public EventHandler
{
	typedef void (HandlerType::*Callback)();

	private:
		std::map<std::string, std::vector<std::pair<HandlerType *, Callback> > > _eventCallbacks;
	public:
		void on(const std::string& event, HandlerType *handler, Callback callback) 
		{
			_eventCallbacks[event].push_back(std::make_pair(handler, callback));
		}

		void emit(const std::string& event)
		{
			if (_eventCallbacks.count(event) == 0)
				return ;

			std::vector<std::pair<HandlerType *, Callback> > &callbacks = _eventCallbacks[event];
			
			for (size_t i = 0; i < callbacks.size(); i++)
			{
				CallbackInvoker<HandlerType> invoker(callbacks[i].first, callbacks[i].second);
				invoker();
			}
		}
		
};

#endif