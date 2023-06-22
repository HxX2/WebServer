/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 17:40:56 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/19 01:10:35 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Modules.hpp>

Logger::Logger() : _DEBUG(DEBUG)
{
    
}

Logger::~Logger()
{
    
}

void Logger::setDebug(bool value)
{
    this->_DEBUG = value;
}

void Logger::log(std::string type, std::string msg)
{
    if (_DEBUG && type == "DEBUG")
        std::cout << GREEN << "[" << type << "] " << RESET << msg << std::endl;
    else if (type == "ERROR")
    {
        std::cerr << RED << "[" << type << "] " << RESET << msg << std::endl;
		if (this->_DEBUG)
			perror("msg");
        exit(1);
    }
    else if (type == "WARNING")
        std::cerr << YELLOW << "[" << type << "] " << RESET << msg << std::endl;
    else if (type == "INFO")
        std::cout << MAGENTA << "[" << type << "] " << RESET << msg << std::endl;
}
