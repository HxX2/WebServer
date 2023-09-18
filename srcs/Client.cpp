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

	_chunk_size = 0;
	_content_length = 0;
	_total_size_read = 0;
	_is_chunked = false;
	_is_first_read = true;
	_is_request_ready = false;
	send_body = false;
	remove_client = false;
	_client_socket = client_fd;
	// should be removed
	_status = "200";
	ret = getsockname(server_fd, (struct sockaddr *)&addr, &addr_len);
	if (!ret)
		_server_address = inet_ntoa(addr.sin_addr);
}

bool extract_line(std::string &buffer, std::string &line, bool include_CRLF = false)
{
	size_t end_i;

	end_i = buffer.find("\r\n");
	if (end_i != std::string::npos)
	{
		line = buffer.substr(0, include_CRLF ? end_i + 2 : end_i);
		buffer.erase(0, end_i + 2);
		return (true);
	}
	line = buffer;
	buffer = "";
	return (false);
}

size_t extract_chunk(std::string &buffer, std::string &chunk, size_t chunk_size)
{
	if (chunk_size == 0)
		return (0);
	else if (chunk_size >= buffer.size())
	{
		chunk = buffer;
		buffer = "";
	}
	else
	{
		chunk = buffer.substr(0, chunk_size);
		buffer.erase(0, chunk_size);
	}
	return (chunk.size());
}

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
	string_buffer.insert(0, buffer, request_len);
	return (request_len);
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
	if (_headers.count("Transfer-Encoding") > 0 && _headers["Transfer-Encoding"] == "chunked")
		_is_chunked = true;
	else if (_headers.count("Content-Length") > 0)
		_content_length = atoi(_headers["Content-Length"].c_str());
}

void Client::set_config(Config &server_config)
{
	_config_directives = server_config.get_config(_server_address, _server_port, _server_name, _path);
}

void Client::create_temp_file()
{
	if (!_temp_file.is_open())
	{
		_temp_file_name = ".tmp";
		utils::time_now(_temp_file_name);
		_temp_file.open(_temp_file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
		if (_temp_file.fail())
			utils::log("ERROR", "couldn't open temp file");
	}
}

void Client::close_temp_file(bool delete_file = true)
{
	_temp_file.close();
	if (delete_file && ::remove(_temp_file_name.c_str()) != 0)
		utils::log("ERROR", "Couldn't delete temp file");
}

void Client::handle_request(Config &server_config)
{
	size_t current_chunk_len = 0;
	std::string string_buffer, temp_line;

	// TODO: check for empty request
	// TODO: handle chunked request

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

	if (!_is_chunked)
	{
		_total_size_read += string_buffer.size();
		if (_total_size_read > _content_length)
			string_buffer.erase(_content_length);
		_temp_file << string_buffer;
		_is_request_ready = (_total_size_read >= _content_length);
	}
	else
	{
		while (!string_buffer.empty())
		{
			if (_chunk_size == 0)
			{
				extract_line(string_buffer, temp_line);
				_chunk_size = utils::hex_to_decimal(temp_line);
				std::cout << _chunk_size << std::endl;
				if (_chunk_size == 0)
				{
					_is_request_ready = true;
					break;
				}
			}
			current_chunk_len = extract_chunk(string_buffer, temp_line, _chunk_size);
			_chunk_size -= current_chunk_len;
			_content_length += current_chunk_len;
			_temp_file << temp_line;
			if (_chunk_size == 0)
			{
				extract_line(string_buffer, temp_line);
				if (!temp_line.empty())
				{
					_is_request_ready = true;
					_status = "400";
					break;
				}
			}
		}
	}
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
		std::cout << "\t" + header_it->first + ": [" + header_it->second + "]\n";
		header_it++;
	}

	std::cout << "Config:\n";
	std::map<std::string, std::string>::iterator config_it = _config_directives.begin();
	while (config_it != _config_directives.end())
	{
		std::cout << "\t" + config_it->first + ": [" + config_it->second + "]\n";
		config_it++;
	}

	std::cout << "Body: \n"
			  << "\t" << (_temp_file_name.empty() ? "[== No Body ==]" : "File name: [" + _temp_file_name + "]")
			  << "\n";
}

void Client::handle_response()
{

	if (_status != "200")
		error_response(_status);
	else if (_config_directives["redirect"] != "")
		redirect_response();
	else
		regular_response();
}

void Client::regular_response()
{
	std::string path = _config_directives["root"] + _path;
	std::string index = path + "/" + _config_directives["index"];
	std::string extension = path.substr(path.find_last_of(".") + 1);

	if (utils::is_dir(path))
	{
		utils::log("DEBUG", "autoindex : \"" + _config_directives["autoindex"] + "\"");
		if (access(index.c_str(), F_OK) != -1 && _config_directives["index"] != "")
			file_response(index, index.substr(index.find_last_of(".") + 1));
		else if (_config_directives["autoindex"] == "true")
			indexer_response(path, _path);
		else
			error_response("403");
	}
	else
	{
		file_response(path, extension);
	}
}

void Client::redirect_response()
{
	this->_version = "HTTP/1.1";
	this->_status = "301";
	this->_headers["Location"] = this->_config_directives["redirect"];
	this->_headers["Content-Length"] = "0";
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";
}

void Client::indexer_response(const std::string &path, const std::string &location)
{
	Templates templates;

	templates.index(path, location);
	this->_version = "HTTP/1.1";
	this->_status = "200";
	this->_body = templates.getIndexerPage();
	this->_headers["Content-Type"] = "text/html";
	this->_headers["Content-Length"] = utils::to_string(this->_body.length());
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";
}

void Client::file_response(std::string path, std::string extension)
{
	_res_file.open(path.c_str(), std::fstream::in | std::fstream::binary);
	std::string mime_type = utils::mimetypes(extension);

	if (!_res_file.good())
	{
		this->error_response("404");
		return;
	}
	if (_res_file.fail())
	{
		this->error_response("403");
		return;
	}
	if (mime_type == "")
		mime_type = "text/plain";

	_res_file.seekg(0, std::fstream::end);
	utils::log("DEBUG", "mime_type : \"" + mime_type + "\"");
	this->_version = "HTTP/1.1";
	this->_status = "200";
	this->_headers["Content-Type"] = mime_type;
	this->_headers["Content-Length"] = utils::to_string(_res_file.tellg());
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";

	std::cout << "file size : " << _headers["Content-Length"] << std::endl;
	std::cout << "file size : " << _res_file.tellg() << std::endl;
	_res_file.seekg(0, std::fstream::beg);
}

void Client::send_response()
{
	std::string raw_response;
	char buffer[BUFFER_SIZE];

	if (!send_body)
	{
		raw_response = this->_version + " " + this->_status + " " + utils::http_msg(_status) + "\r\n";

		for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
			raw_response += it->first + ": " + it->second + "\r\n";

		raw_response += "\r\n";
		send(_client_socket, raw_response.c_str(), raw_response.length(), 0);
		send_body = true;
	}
	else if (_res_file.is_open())
	{
		_res_file.read(buffer, BUFFER_SIZE);
		send(_client_socket, buffer, _res_file.gcount(), 0);
		if (_res_file.eof())
		{
			_res_file.close();
			remove_client = true;
			log_response();
		}
	}
	else
	{
		send(_client_socket, this->_body.c_str(), this->_body.length(), 0);
		remove_client = true;
		log_response();
	}
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
	if (_status[0] == '2')
		_status = GREEN + _status;
	else if (_status[0] == '3')
		_status = MAGENTA + _status;
	else if (_status[0] == '4')
		_status = YELLOW + _status;
	else
		_status = RED + _status;

	std::cout << BLUE << "[RESPONSE] " << RESET << "(" << utils::http_date() << ")" << GREEN << " HTTP : " << RESET << this->_version
			  << " " + _path + " " << this->_status << RESET << "\n"
			  << std::endl;
}
