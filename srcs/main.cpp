#include "Config.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "ServersManager.hpp"

int main(int argc, const char *argv[])
{
	std::string config_path;

	try
	{
		config_path = argc == 2 ? argv[1] : "configs/webserv.conf";
		Config conf(config_path);
		// std::cout << conf << std::endl;
		ServersManager manager(conf);
		manager.startServers();
	}
	catch (const std::exception &error)
	{
		std::cerr << RED << "🛑 Error: " << RESET << error.what() << '\n';
	}
	catch (...)
	{
		std::cerr << RED << "🛑 Error: " << RESET << "Unknown error\n";
	}

	return (0);
}
