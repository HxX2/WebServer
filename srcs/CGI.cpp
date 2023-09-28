#include <CGI.hpp>
#include <algorithm>

CGI::CGI()
{
}

CGI::CGI(std::string host, std::string port, std::string uri, std::string query) : pid(-1), _host(host), _port(port), _uri(uri), _query(query)
{
}

CGI::~CGI()
{
}

void CGI::set_path_info(std::string script)
{
	_path_info = _uri.substr(_uri.find_last_of(script) + 1);
}

void CGI::set_meta_variables(str_map_t header, std::string method, std::string version, std::string script_path)
{
	_env_variables["SCRIPT_FILENAME"] = script_path;
	_env_variables["CONTENT_LENGTH"] = header["Content-Length"];
	_env_variables["CONTENT_TYPE"] = header["Content-Type"];
	_env_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env_variables["PATH_INFO"] = _path_info;
	_env_variables["QUERY_STRING"] = _query;
	_env_variables["REQUEST_METHOD"] = method;
	_env_variables["REQUEST_URI"] = _uri;
	_env_variables["SERVER_NAME"] = _host;
	_env_variables["SERVER_PORT"] = _port;
	_env_variables["SERVER_PROTOCOL"] = version;
	_env_variables["REDIRECT_STATUS"] = "TRUE";
	for (str_map_t::iterator it = header.begin(); it != header.end(); it++)
	{
		if (it->first == "Content-Length" || it->first == "Content-Type")
			continue;
		std::string key = "HTTP_" + it->first;
		std::string value = it->second;
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		_env_variables[key] = value;
	}
	std::cout << "uri" << _uri << std::endl;
	std::cout << _env_variables["SCRIPT_NAME"] << std::endl;
}

char **CGI::get_envp()
{
	char **envp = new char *[_env_variables.size() + 1];
	int i = 0;

	for (str_map_t::iterator it = _env_variables.begin(); it != _env_variables.end(); it++)
	{
		envp[i] = new char[it->first.size() + it->second.size() + 2];
		strcpy(envp[i], (it->first + "=" + it->second).c_str());
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void CGI::delete_envp(char **envp)
{
	for (int i = 0; envp[i]; i++)
		delete[] envp[i];
	delete[] envp;
}