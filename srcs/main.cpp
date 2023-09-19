#include "Config.hpp"
#include "Request.hpp"
#include <ServersManager.hpp>

int main(int argc, const char *argv[])
{
	Config cnf;
	ServersManager manager;

	if (argc != 2)
		std::cerr << "Usage: ./webserv [config_file.conf]" << std::endl;
	else
	{
		try
		{
			cnf.read(argv[1]);
			manager.loadConfig(cnf);
			manager.startServers();
		}
		catch (const std::exception &e)
		{
			std::cerr << RED << "🛑 Error: " << RESET << e.what() << '\n';
		}
	}

	return (0);
}
