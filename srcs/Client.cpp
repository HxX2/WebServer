#include <Client.hpp>

Client::Client()
{
}

Client::~Client()
{
}

Client::Client(int client_fd, int server_fd)
{
	struct sockaddr_in addr;
	int ret;
	socklen_t addr_len = sizeof(addr);

	_is_request_ready = false;
	_client_socket = client_fd;
	ret = getsockname(server_fd, (struct sockaddr *)&addr, &addr_len);
	if (!ret)
		_server_address = inet_ntoa(addr.sin_addr);
}

void Client::handle_request()
{
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
