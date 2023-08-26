#include "Server.hpp"
#include "Config.hpp"

void	log(std::string type, std::string msg)
{
	std::cout << "[" << type << "] " << msg << std::endl;
	if (type == "ERROR") exit(EXIT_FAILURE);
}

size_t	create_socket(size_t address, size_t port)
{
	int					opt = 1;
	ssize_t				socket_fd;
	struct sockaddr_in	socket_addr;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		log("ERROR", "Failed to create the socket");

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
		log("ERROR", "Failed to make socket non blocking");

	memset(&socket_addr, 0, sizeof(socket_addr));
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(port);
	socket_addr.sin_addr.s_addr = htonl(address);

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		log("ERROR", "Failed to set socket options");

	if (bind(socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
		log("ERROR", "Failed to bind socket to port");

	if (listen(socket_fd, SOMAXCONN) < 0)
		log("ERROR", "failed to accept connection on socket");

	std::cout << "🚀 Server listening on http://localhost:" << port << std::endl;

	return (socket_fd);
}

void handle_new_conn(size_t socket_fd, fd_set *main_fds, size_t *max_fds)
{
	ssize_t				client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_size;

	client_fd = accept(socket_fd, (sockaddr *)&client_addr, &client_addr_size);
	if (client_fd < 0)
		log("ERROR", "Failed to create the client socket");
	std::cout << "👋 Received new connection: " << client_fd << std::endl;
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
		log("ERROR", "Failed to set client socket flags");
	FD_SET(client_fd, main_fds);
	*max_fds = (size_t)client_fd > *max_fds ? client_fd : *max_fds;
}

void mock_server(void (*handle_request)(size_t, char *, Config &conf), Config &conf)
{
	size_t		socket_fd, max_fds, ready_fd;
	fd_set		main_fds, working_fds;
	char		buffer[100000];

	socket_fd = create_socket(INADDR_ANY, 8080);

	FD_ZERO(&main_fds);
	max_fds = socket_fd + 1;
	FD_SET(socket_fd, &main_fds);

	while (true)
	{
		working_fds = main_fds;
		bzero(buffer, sizeof(buffer));

		if ((ready_fd = select(FD_SETSIZE, &working_fds, NULL, NULL, NULL)) <= 0)
			log("ERROR", "Failed to select");

		for (size_t i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &working_fds))
			{
				if (i == socket_fd)
					handle_new_conn(socket_fd, &main_fds, &max_fds);
				else
				{
					recv(i, buffer, sizeof(buffer), 0);
					// std::cout << "[REQUEST]\n" << buffer << std::endl;
					handle_request(socket_fd, buffer, conf);
					send(i, "HTTP/1.1 200 OK\r\n\r\nWell received\r\n", 33, 0);
					FD_CLR(i, &main_fds);
					close(i);
				}
			}
		}
	}
}
