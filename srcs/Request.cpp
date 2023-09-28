#include "Client.hpp"

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

ssize_t Client::read_buffer(std::string &string_buffer)
{
	int request_len;
	char buffer[BUFFER_SIZE + 1];

	memset(buffer, 0, sizeof(buffer));
	request_len = recv(_client_socket, buffer, BUFFER_SIZE, 0);
	if (request_len <= 0)
		return (-1);
	string_buffer.insert(0, buffer, request_len);
	return (request_len);
}

bool Client::is_path_valid(std::string &path)
{
	std::string valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;%=";
	for (size_t i = 0; i < path.size(); i++)
		if (valid_chars.find(path[i]) == std::string::npos)
			return (false);
	return (true);
}

bool Client::is_allowed_method(std::string &method)
{
	std::vector<std::string> allowed_methods;

	if (_config_directives.count("allowed_methods") > 0)
		allowed_methods = utils::split_str(_config_directives["allowed_methods"], ' ');
	else
		return (true);
	for (size_t i = 0; i < allowed_methods.size(); i++)
		if (allowed_methods[i] == method)
			return (true);
	return (false);
}

bool Client::handle_error(std::string status_code)
{
	_status = status_code;
	_is_request_ready = true;
	return (false);
}

bool Client::set_request_line(std::string &line)
{
	utils::t_str_arr tokens;

	tokens = utils::split_str(line, ' ');
	if (tokens.size() != 3)
		return (handle_error("400"));
	else
	{
		_method = tokens[0];
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
			return (handle_error("405"));
		_path = tokens[1];
		if (!is_path_valid(_path))
			return (handle_error("400"));
		size_t params_i = _path.find_first_of("?");
		if (params_i != std::string::npos)
		{
			_params = _path.substr(params_i + 1);
			_path = _path.erase(params_i);
		}
		_version = tokens[2];
		if (_version != "HTTP/1.1")
			return (handle_error("505"));
	}
	return (true);
}

bool Client::set_header(std::string &line)
{
	utils::t_str_arr header;

	header = utils::split_str(line, ": ");
	if (header.size() == 2)
		_headers[header[0]] = header[1];
	else
		return (handle_error("400"));
	return (true);
}

bool Client::set_host_info()
{
	if (_headers.count("Host") == 0)
		return (handle_error("400"));
	else
	{
		utils::t_str_arr split_host = utils::split_str(_headers["Host"], ':');
		if (split_host.size() == 1 || split_host.size() == 2)
			_server_name = split_host[0];
		else
			return (handle_error("400"));
	}
	return (true);
}

bool Client::set_request_size()
{
	if (_method != "POST")
		return (true);
	if (_headers.count("Transfer-Encoding") > 0)
	{
		if (_headers["Transfer-Encoding"] == "chunked")
			_is_chunked = true;
		else
			return (handle_error("501"));
	}
	else if (_headers.count("Content-Length") > 0)
	{
		_content_length = atoi(_headers["Content-Length"].c_str());
		if (_content_length == 0 && _headers["Content-Length"] != "0")
			return (handle_error("411"));
		if (_config_directives.count("max_body_size") > 0)
		{
			unsigned long max_body_size = atoi(_config_directives["max_body_size"].c_str());
			if (_content_length > max_body_size)
				return (handle_error("413"));
		}
	}
	else
		return (handle_error("411"));
	return (true);
}

bool Client::set_config(Config &server_config)
{
	_original_path = _path;
	_config_directives = server_config.get_config(_server_address, _server_port, _server_name, _path);
	if (!is_allowed_method(_method))
		return (handle_error("405"));
	return (true);
}

bool Client::create_temp_file()
{
	if (!_temp_file.is_open())
	{
		_temp_file_name = ".tmp";
		utils::time_now(_temp_file_name);
		_temp_file.open(_temp_file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
		if (_temp_file.fail())
		{
			utils::log("ERROR", "couldn't open temp file");
			remove_client = true;
			_is_request_ready = true;
			send_body = true;
			return (false);
		}
	}
	return (true);
}

void Client::close_temp_file(bool delete_file = true)
{
	_temp_file.close();
	if (!access(_temp_file_name.c_str(), F_OK))
		return;
	if (delete_file && ::remove(_temp_file_name.c_str()) != 0)
	{
		utils::log("ERROR", "Couldn't delete temp file");
		remove_client = true;
		_is_request_ready = true;
		send_body = true;
	}
}

void Client::handle_request(Config &server_config)
{
	size_t current_chunk_len = 0;
	std::string string_buffer, temp_line;

	if (read_buffer(string_buffer) == -1)
	{
		send_body = true;
		_is_request_ready = true;
		remove_client = true;
		return;
	}

	_is_request_ready = true;

	if (_is_first_read)
	{
		if (string_buffer == "\r\n")
		{
			handle_error("400");
			return;
		}

		while (extract_line(string_buffer, temp_line))
			if (!temp_line.empty())
				break;

		if (!set_request_line(temp_line))
			return;

		while (extract_line(string_buffer, temp_line))
		{
			if (temp_line.empty())
				break;
			if (!set_header(temp_line))
				return;
		}

		if (!set_host_info())
			return;
		if (!set_config(server_config))
			return;
		if (!set_request_size())
			return;
		if (!create_temp_file())
			return;

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
				if (_chunk_size == 0)
				{
					_is_request_ready = true;
					break;
				}
			}
			current_chunk_len = extract_chunk(string_buffer, temp_line, _chunk_size);
			_chunk_size -= current_chunk_len;
			_content_length += current_chunk_len;
			if (_config_directives.count("max_body_size") > 0)
			{
				unsigned long max_body_size = atoi(_config_directives["max_body_size"].c_str());
				if (_content_length > max_body_size)
				{
					handle_error("413");
					return;
				}
			}
			_temp_file << temp_line;
			if (_chunk_size == 0)
			{
				extract_line(string_buffer, temp_line);
				if (!temp_line.empty())
				{
					handle_error("400");
					return;
				}
			}
		}
	}
}

void Client::log_reuqest()
{
	std::cout << "Address: [" << _server_address << "], "
			  << "Port: [" << _server_port << "], "
			  << "Server_Name: [" << _server_name << "]" << std::endl;

	std::cout
		<< "Method: [" + _method + "], "
		<< "Path: [" + _path + "], "
		<< "Params: [" + _params + "], "
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
