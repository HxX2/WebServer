/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:44:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/05/29 21:38:55 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

void EventEmitter::on(std::string& event,EventHandler *handler, Callback callback)
{
	_eventCallbacks[event].push_back(std::make_pair(handler, callback));
}

void EventEmitter::emit(std::string& event)
{
	if (_eventCallbacks.count(event) == 0)
		return ;

	std::vector<std::pair<EventHandler *, Callback> > &callbacks = _eventCallbacks[event];
	
	for (size_t i = 0; i < callbacks.size(); i++)
	{
		CallbackInvoker invoker(callbacks[i].first, callbacks[i].second);
		invoker();
	}
}

CallbackInvoker::CallbackInvoker(EventHandler *handler, Callback callback) : _handler(handler), _callback(callback)
{
	
}

void CallbackInvoker::operator()()
{
	(_handler->*_callback)();
}
