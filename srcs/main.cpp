#include "Config.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "ServersManager.hpp"

int main(int argc, const char *argv[])
{
	std::string config_path;

	try
	{
		config_path = argc == 2 ? argv[1] : "config/webserv.conf";
		Config cnf(config_path);
		// ServersManager manager(cnf);
		// manager.startServers();
		std::cout << cnf;
	}
	catch (const std::exception &error)
	{
		std::cerr << RED << "🛑 Error: " << RESET << error.what() << '\n';
	}

	return (0);
}
