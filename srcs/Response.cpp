#include "Client.hpp"

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
