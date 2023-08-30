
#ifndef __REQUEST_HPP_
#define __REQUEST_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <ctime>

#include <Config.hpp>
#include <Indexer.hpp>
#include <utils.hpp>

class Client
{
private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _status;

	std::string _server_address;
	std::string _server_name;
	size_t _server_port;
	std::string _raw_response;

	std::map<std::string, std::string> _config_directives;

	bool _is_request_ready = false;

public:
	int _client_socket;

	Client();
	Client(int client_socket);
	~Client();

	void handle_request();
	void handle_response();
	void indexer_response(std::string path);
	void send_response();
	void log_response();
};

#endif
