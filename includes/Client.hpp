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
#include <CGI.hpp>

class Client
{
private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _status;
	std::ifstream _res_file;
	CGI *_cgi;
	int _pipe[2];

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
	int _client_socket;

	Client();
	Client(int client_fd, int server_fd);
	~Client();

	const std::string &get_body() const { return (_body); }

	void handle_request(Config &server_config);
	size_t read_buffer(std::string &string_buffer);
	void set_request_line(std::string &line);
	void set_header(std::string &line);
	void set_host_info();
	void set_request_size();
	void set_config(Config &server_config);
	void log_reuqest();
	void create_temp_file();
	void close_temp_file(bool delete_file);

	void handle_response();
	void indexer_response(const std::string &path, const std::string &location);
	void redirect_response();
	void send_response();
	void log_response();
	void error_response(std::string status);
	void delete_response(std::string file_name);
	void post_response();
	void file_response(std::string path, std::string extension);
	void regular_response();
	std::string get_index(std::string path);
	void create_cgi_file();
	void cgi_response();
	void exec_cgi();
	int wait_cgi();
};

#endif
