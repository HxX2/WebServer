#include "Config.hpp"
#include "Request.hpp"

void mock_server(void (*handle_request)(size_t, char *, Config &conf), Config &conf);

void handle_request(size_t socket_fd, char *buffer, Config &server_config)
{
	// Request req(socket_fd, buffer);
	// req.parseRequest(server_config);
	(void)socket_fd;
	(void)server_config;
	std::cout << buffer << std::endl;
}

int main(int argc, const char *argv[])
{
	Config cnf;

	if (argc != 2)
		std::cerr << "Usage: ./webserv [config_file.conf]" << std::endl;
	else
	{
		try
		{
			cnf.read(argv[1]);
			mock_server(handle_request, cnf);
		}
		catch (const std::exception &e)
		{
			std::cerr << RED << "🛑 Error: " << RESET << e.what() << '\n';
		}
	}

	return (0);
}
