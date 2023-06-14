/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 17:40:56 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/12 15:36:16 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Logger.hpp>

Logger::Logger() : _DEBUG(false)
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
        exit(1);
    }
    else if (type == "WARNING")
        std::cerr << YELLOW << "[" << type << "] " << RESET << msg << std::endl;
    else if (type == "INFO")
        std::cout << MAGENTA << "[" << type << "] " << RESET << msg << std::endl;
}
