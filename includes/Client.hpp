#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_

#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <ctime>

#include <Config.hpp>
#include <Templates.hpp>
#include <utils.hpp>

#define BUFFER_SIZE 10000

class Client
{
private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::ifstream _res_file;

	std::string _server_address;
	std::string _server_name;
	size_t _server_port;

	std::map<std::string, std::string> _config_directives;

	std::fstream _temp_file;
	std::string _temp_file_name;

	bool _is_first_read;
	unsigned long _content_length;
	unsigned long _total_size_read;

	bool _is_chunked;
	long _chunk_size;

public:
	bool _is_request_ready;
	bool remove_client;
	bool send_body;
	std::string _status;
	int _client_socket;

	Client();
	Client(int client_fd, int server_fd);
	~Client();

	void handle_request(Config &server_config);
	size_t read_buffer(std::string &string_buffer);
	bool set_request_line(std::string &line);
	bool set_header(std::string &line);
	bool set_host_info();
	bool set_request_size();
	bool set_config(Config &server_config);
	void create_temp_file();
	void close_temp_file(bool delete_file);
	bool is_path_valid(std::string &path);
	bool is_allowed_method(std::string &method);
	bool handle_error(std::string status_code);
	void log_reuqest();

	void handle_response();
	void indexer_response(const std::string &path, const std::string &location);
	void redirect_response();
	void send_response();
	void log_response();
	void error_response(std::string status);
	void file_response(std::string path, std::string extension);
	void regular_response();
};

#endif
