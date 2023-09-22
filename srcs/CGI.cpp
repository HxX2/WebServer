#include <CGI.hpp>

CGI::CGI()
{
}

CGI::CGI(std::string host, std::string port, std::string uri) : pid(-1), _host(host), _port(port) , _uri(uri)
{
}

CGI::~CGI()
{
}

void CGI::set_path_info(std::string script)
{
	_path_info = _uri.substr(_uri.find_last_of(script) + 1);
}

void CGI::set_meta_variables(str_map_t header, std::string method, std::string version)
{
	_meta_variables["CONTENT_LENGTH"] = header["Content-Length"];
	_meta_variables["CONTENT_TYPE"] = header["Content-Type"];
	_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	_meta_variables["PATH_INFO"] = _path_info;
	_meta_variables["QUERY_STRING"] = _uri.substr(_uri.find('?') + 1);
	_meta_variables["REQUEST_METHOD"] = method;
	_meta_variables["REQUEST_URI"] = _uri;
	_meta_variables["SERVER_NAME"] = _host;
	_meta_variables["SERVER_PORT"] = _port;
	_meta_variables["SERVER_PROTOCOL"] = version;
	_meta_variables["REDIRECT_STATUS"] = "TRUE";
}

char **CGI::get_envp()
{
	char **envp = new char*[_meta_variables.size() + 1];
	int i = 0;

	for (str_map_t::iterator it = _meta_variables.begin(); it != _meta_variables.end(); it++)
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
		delete [] envp[i];
	delete [] envp;
}