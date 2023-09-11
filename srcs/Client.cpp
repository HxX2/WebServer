#include <Client.hpp>

#define BUFFER_SIZE 10000

Client::~Client()
{
}

Client::Client(int client_fd, int server_fd)
{
	struct sockaddr_in addr;
	int ret;
	socklen_t addr_len = sizeof(addr);

	_content_length = 0;
	_total_size_read = 0;
	_is_chunked = false;
	_is_first_read = true;
	_is_request_ready = false;
	_client_socket = client_fd;
	ret = getsockname(server_fd, (struct sockaddr *)&addr, &addr_len);
	if (!ret)
		_server_address = inet_ntoa(addr.sin_addr);
}

bool extract_line(std::string &str, std::string &line)
{
	size_t end_i;

	end_i = str.find("\r\n");
	if (end_i != std::string::npos)
	{
		line = str.substr(0, end_i);
		str.erase(0, end_i + 2);
		return (true);
	}
	line = str;
	str = "";
	return (false);
}

void Client::set_request_line(std::string &line)
{
	utils::t_str_arr tokens;

	// TODO: check for invalid request line
	tokens = utils::split_str(line, ' ');
	if (tokens.size() == 3)
	{
		// TODO: check for invalid method from config
		_method = tokens[0];
		// TODO: check for invalid path
		_path = tokens[1];
		// TODO: check for invalid version
		_version = tokens[2];
	}
}

void Client::set_header(std::string &line)
{
	utils::t_str_arr header;

	// TODO: check for invalid headers (empty_value)
	header = utils::split_str(line, ": ");
	if (header.size() == 2)
		_headers[header[0]] = header[1];
}

void Client::set_host_info()
{
	// TODO: check for host not found
	if (_headers.count("Host") > 0)
	{
		utils::t_str_arr split_host = utils::split_str(_headers["Host"], ':');
		// TODO: check for invalid value in host
		if (split_host.size() == 2)
		{
			_server_name = split_host[0];
			// TODO: check for atoi errors
			_server_port = atoi(split_host[1].c_str());
		}
	}
}

void Client::set_request_size()
{
	// TODO: check for requests without Transfer-encoding & Content-Length header
	// TODO: check if value isn't chunked
	// TODO: check for 0 content_length
	if (_headers.count("Transfer-encoding") > 0 && _headers["Transfer-encoding"] == "chunked")
		_is_chunked = true;
	else if (_headers.count("Content-Length") > 0)
		_content_length = atoi(_headers["Content-Length"].c_str());
}

void Client::set_config(Config &server_config)
{
	_config_directives = server_config.get_config(_server_address, _server_port, _server_name, _path);
}

// TODO: handle sys calls failure
size_t Client::read_buffer(std::string &string_buffer)
{
	int request_len;
	char buffer[BUFFER_SIZE + 1];

	memset(buffer, 0, sizeof(buffer));
	request_len = recv(_client_socket, buffer, BUFFER_SIZE, 0);
	if (request_len < 0)
		utils::log("ERROR", "failed to read request");
	else if (_is_first_read && request_len == 0)
		utils::log("ERROR", "request cannot be empty");
	string_buffer.assign(buffer);
	return (request_len);
}

void Client::create_temp_file()
{
	std::string name = ".tmp";

	utils::time_now(name);
	temp_file.open(name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	if (temp_file.fail())
		utils::log("ERROR", "couldn't open temp file");
}

// TODO: check for empty request
// TODO: handle chunked request
void Client::handle_request(Config &server_config)
{
	std::string string_buffer, temp_line;

	if (_is_request_ready)
		return;

	read_buffer(string_buffer);

	if (_is_first_read)
	{
		while (extract_line(string_buffer, temp_line))
			if (!temp_line.empty())
				break;

		// TODO: check for invalid request line (empty, invalid token)
		set_request_line(temp_line);

		// TODO: check for empty headers
		while (extract_line(string_buffer, temp_line))
		{
			if (temp_line.empty())
				break;
			set_header(temp_line);
		}

		set_host_info();
		set_request_size();
		set_config(server_config);
		create_temp_file();

		_is_first_read = false;
	}

	_total_size_read += string_buffer.size();

	if (_total_size_read > _content_length)
		string_buffer.erase(_content_length);

	temp_file << string_buffer;

	_is_request_ready = (_total_size_read >= _content_length);

	// if (_is_request_ready)
	// 	log_reuqest();
}

void Client::log_reuqest()
{
	std::cout << "Request from: " << _server_address << ", "
			  << "on port: " << _server_port << ", "
			  << "domain name: " << _server_name << std::endl;

	std::cout
		<< "Method: [" + _method + "], "
		<< "Path: [" + _path + "], "
		<< "Version: [" + _version + "]" << std::endl;

	std::cout << "Headers:\n";
	std::map<std::string, std::string>::iterator header_it = _headers.begin();
	while (header_it != _headers.end())
	{
		std::cout << "\t[" + header_it->first + "]: [" + header_it->second + "]\n";
		header_it++;
	}

	std::cout << "Config:\n";
	std::map<std::string, std::string>::iterator config_it = _config_directives.begin();
	while (config_it != _config_directives.end())
	{
		std::cout << "\t[" + config_it->first + "]: [" + config_it->second + "]\n";
		config_it++;
	}

	std::cout << "Body [" << _body.size() << "]:\n";
	std::cout << "\t" << (_body.size() > 0 ? _body : "~~ Empty body ~~") << std::endl;
}

void Client::handle_response()
{
}

void Client::indexer_response(std::string path)
{
	Templates templates;

	templates.index(path);
	this->_version = "HTTP/1.1";
	this->_status = "200";
	this->_body = templates.getIndexerPage();
	this->_headers["Content-Type"] = "text/html";
	this->_headers["Content-Length"] = utils::to_string(this->_body.length());
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";
}

void Client::send_response()
{
	std::string raw_response;

	raw_response = this->_version + " " + this->_status + " " + utils::http_msg(_status) + "\r\n";

	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		raw_response += it->first + ": " + it->second + "\r\n";

	raw_response += "\r\n";
	raw_response += this->_body;

	::send(_client_socket, raw_response.c_str(), raw_response.length(), 0);
}

void Client::error_response(std::string status)
{
	std::string path = this->_config_directives["error_page_" + status];
	Templates templates;

	this->_version = "HTTP/1.1";
	this->_status = status;
	this->_body = templates.getErrorPage(status, path);
	this->_headers["Content-Type"] = "text/html";
	this->_headers["Content-Length"] = utils::to_string(this->_body.length());
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";
}

void Client::log_response()
{
	std::cout << BLUE << "[RESPONSE] " << RESET << "(" << utils::http_date() << ")" << GREEN << " HTTP : " << RESET << this->_version
			  << " / " << GREEN << this->_status << RESET << "\n"
			  << std::endl;
}
