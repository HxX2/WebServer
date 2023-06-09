/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:29:40 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/07 15:06:41 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <ctime>

class Response
{
	private:
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _status;
		std::string _body;
		std::string _version;
		std::string _buffer;
	public:
		Response();
		~Response();
		
		void setVersion(const std::string& version);
		void setStatus(const std::string& code, const std::string& name);
		void setHeader(const std::string& key, const std::string& value);
		void setBody(const std::string& body);
		void send (int socket);

		static std::string getCurrentDate();
};

#endif