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

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

class Logger
{
private:
    bool _DEBUG;

public:
    Logger();
    ~Logger();

    void setDebug(bool value);
    void log(std::string type, std::string msg);
};

#endif
