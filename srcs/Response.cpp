#include "Client.hpp"

void Client::handle_response()
{
	if (_status != "200")
		error_response(_status);
	else if (_config_directives["redirect"] != "")
		redirect_response();
	else if (_method == "POST")
		post_response();
	else
		regular_response();
}

std::string Client::get_index(std::string path)
{
	utils::t_str_arr indexes = utils::split_str(_config_directives["index"], ' ');

	for (utils::t_str_arr::iterator it = indexes.begin(); it != indexes.end(); ++it)
	{
		std::string index = path + "/" + *it;
		if (access(index.c_str(), F_OK) != -1 && !utils::is_dir(index))
			return (index);
	}
	return ("");
}

void Client::regular_response()
{
	std::string path = _config_directives["root"] + _path.substr(0, _path.find_last_of("?"));
	std::string index = get_index(path);
	std::string extension = path.substr(path.find_last_of(".") + 1);

	if (utils::is_dir(path))
	{
		utils::log("DEBUG", "autoindex : \"" + _config_directives["autoindex"] + "\"");
		if (index != "")
		{
			utils::log("DEBUG", "index : \"" + index + "\"");
			if (_config_directives["cgi_" + index.substr(path.find_last_of(".") + 1)] != "")
				cgi_response();
			else
				file_response(index, index.substr(index.find_last_of(".") + 1));
		}
		else if (_config_directives["autoindex"] == "true")
		{
			utils::log("DEBUG", _path);
			indexer_response(path, _path);
		}
		else
			error_response("403");
	}
	else
	{
		if (_method == "DELETE")
			delete_response(path);
		else if (_config_directives["cgi_" + extension] != "")
			cgi_response();
		else
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
	std::string mime_type = utils::mimetypes(extension, false);

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
	int status;
	size_t ret;

	if (_cgi != NULL && _status == "200")
	{
		status = wait_cgi();

		ret = read(_pipe[0], buffer, BUFFER_SIZE);
		buffer[ret] = '\0';
		utils::log("DEBUG", "buffer : \"" + std::string(buffer) + "\"");
		utils::log("DEBUG", "ret : \"" + utils::to_string(ret) + "\"");
		send(_client_socket, buffer, ret, 0);
		if (!ret && status)
		{
			close(_pipe[0]);
			remove_client = true;
			log_response();
		}
		send_body = true;
	}
	else if (!send_body)
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

void Client::delete_response(std::string file_name)
{
	std::cout << "file_name : " << file_name << std::endl;
	if (!access(file_name.c_str(), W_OK))
		remove(file_name.c_str());
	else
	{
		error_response("500");
		return;
	}

	this->_version = "HTTP/1.1";
	this->_status = "204";
	this->_body = "";
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";
}

void Client::post_response()
{
	std::string upload_dir = _config_directives["upload_dir"];
	std::string file_name;

	if (utils::is_dir(upload_dir) && access(upload_dir.c_str(), W_OK))
	{
		close_temp_file(true);
		error_response("500");
		return;
	}
	file_name = upload_dir + _temp_file_name.substr(0, _temp_file_name.find_last_of(".")) + utils::mimetypes(_headers["Content-Type"], true);
	rename(_temp_file_name.c_str(), file_name.c_str());

	this->_version = "HTTP/1.1";
	this->_status = "201";
	this->_body = "";
	this->_headers["Date"] = utils::http_date();
	this->_headers["Server"] = "Webserv";
}

void Client::cgi_response()
{
	_cgi = new CGI(_server_name, utils::to_string(_server_port), _path);
	_cgi->file_path = _config_directives["root"] + _path;
	_cgi->extension = _path.substr(_path.find_last_of(".") + 1);
	_cgi->path = _config_directives["cgi_" + _cgi->extension];

	_cgi->set_path_info(_cgi->extension);
	_cgi->set_meta_variables(_headers, _method, _version);

	// utils::log("DEBUG", "cgi_path : \"" + _cgi->path + "\"");
	// utils::log("DEBUG", "extension : \"" + _cgi->extension + "\"");
	exec_cgi();
	wait_cgi();
}

void Client::exec_cgi()
{
	pipe(_pipe);
	_cgi->pid = fork();
	if (_cgi->pid == 0)
	{
		char **envp = _cgi->get_envp();
		char *argv[] = {
			const_cast<char *>(_cgi->path.c_str()),
			const_cast<char *>(_cgi->file_path.c_str()),
			NULL};

		int fd = open(_temp_file_name.c_str(), O_RDONLY);
		dup2(fd, 0);
		close(fd);
		close(_pipe[0]);
		dup2(_pipe[1], 1);
		close(_pipe[1]);
		execve(_cgi->path.c_str(), argv, envp);
		_cgi->delete_envp(envp);
		exit(1);
	}
	else
		close(_pipe[1]);
}

int Client::wait_cgi()
{
	int status = 0;

	if (!waitpid(_cgi->pid, &status, WNOHANG))
		return 0;
	else
		return 1;

	if (WEXITSTATUS(status) > 0)
	{
		utils::log("DEBUG", "CGI exited with status : " + utils::to_string(WEXITSTATUS(status)));
		error_response("500");
	}
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
