#ifndef _CGI_HPP_
#define _CGI_HPP_

#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

class CGI
{
public:
	typedef std::map<std::string, std::string> str_map_t;
	pid_t pid;
	std::string path;
	std::string extension;
	std::string file_path;

private:
	std::map<std::string, std::string> _env_variables;
	std::string _path_info;
	std::string _host;
	std::string _port;
	std::string _uri;
	std::string _query;

public:
	CGI();
	CGI(std::string host, std::string port, std::string uri, std::string query);
	~CGI();

	void set_meta_variables(str_map_t header, std::string method, std::string version);
	void set_path_info(std::string script);
	char **get_envp();
	void delete_envp(char **envp);
};

#endif