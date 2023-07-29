#include "Config.hpp"
#include "Request.hpp"

int main(int argc, const char *argv[])
{
	// Config cnf;
	// if (argc != 2)
	// 	std::cerr << "Usage: ./webserv [config_file.conf]" << std::endl;
	// else
	// {
	// 	try
	// 	{
	// 		cnf.read(argv[1]);
	// 		std::cout << cnf;
	// 	}
	// 	catch (const std::exception &e)
	// 	{
	// 		std::cerr << RED << "🛑 Error: " << RESET << e.what() << '\n';
	// 	}
	// }

	(void)argc; (void)argv;
	char b[] = "one\r\ntwo\r\nthree\r\n";
	Request req(b);

	return (0);
}
