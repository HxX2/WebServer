#include "Server.hpp"
#include "EventHandler.hpp"
#include "Config.hpp"

void print_error(std::string error)
{
	std::cerr << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	Config cnf;

	if (argc != 2)
		std::cerr << "Usage: ./webserv [config_file.conf]" << std::endl;
	else
	{
		try
		{
			cnf.read(argv[1]);
			std::cout << cnf;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return (0);
}
