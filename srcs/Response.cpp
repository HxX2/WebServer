/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:45:49 by zlafou            #+#    #+#             */
/*   Updated: 2023/06/06 13:31:37 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Response.hpp>

Response::Response()
{
	
}

Response::~Response()
{
	
}

void Response::setVersion(const std::string& version)
{
	this->_version = version;
}

void Response::setStatus(const std::string& code, const std::string& name)
{
	this->_status[code] = name;	
}

void Response::setHeader(const std::string& key, const std::string& value)
{
	this->_headers[key] = value;
}

void Response::setBody(const std::string& body)
{
	this->_body = body;
}

void Response::send(int socket)
{
	this->_buffer = this->_version + " " + this->_status.begin()->first + " " + this->_status.begin()->second + "\r\n";
	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		this->_buffer += it->first + ": " + it->second + "\r\n";
	this->_buffer += "\r\n";
	this->_buffer += this->_body;
	
	::send(socket, this->_buffer.c_str(), this->_buffer.length(), 0);
}

std::string Response::getCurrentDate()
{
	std::time_t t = std::time(0);
	std::tm* now = std::gmtime(&t);

	const char* format = "%a, %d %b %Y %H:%M:%S GMT";
	char buffer[64];

	std::strftime(buffer, sizeof(buffer), format, now);

	return (std::string(buffer));
}

