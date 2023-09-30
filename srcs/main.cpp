#include "Config.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "ServersManager.hpp"

int main(int argc, const char *argv[])
{
	std::string config_path;
	Config conf;

	signal(SIGPIPE, SIG_IGN);
	try
	{
		config_path = argc == 2 ? argv[1] : "configs/webserv.conf";
		conf.read_config(config_path);
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
