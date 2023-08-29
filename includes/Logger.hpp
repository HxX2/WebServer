/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cipher <cipher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 17:36:48 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/22 13:37:29 by cipher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <string>
#include <cstdlib>

#include "general.hpp"

class Logger
{
private:
	bool _DEBUG;

public:
	Logger();
	~Logger();

	void setDebug(bool value);
	static void log(std::string type, std::string msg);
};

#endif
