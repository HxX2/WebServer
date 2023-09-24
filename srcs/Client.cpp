#include <Client.hpp>

Client::Client(int client_fd, int server_fd)
{
	struct sockaddr_in addr;
	int ret;
	socklen_t addr_len = sizeof(addr);

	_status = "200";
	_is_first_read = true;
	_is_chunked = false;
	_chunk_size = 0;
	_content_length = 0;
	_total_size_read = 0;
	_is_request_ready = false;
	send_body = false;
	remove_client = false;
	_client_socket = client_fd;
	_cgi = NULL;
	ret = getsockname(server_fd, (struct sockaddr *)&addr, &addr_len);
	if (!ret)
	{
		_server_address = inet_ntoa(addr.sin_addr);
		_server_port = ntohs(addr.sin_port);
	}
}

Client::~Client()
{
}
