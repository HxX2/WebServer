#include "Request.hpp"

/**
 * Request default constructor
 */
Request::Request(void) {}

/**
 * Request constructor
 *
 * @param buffer request buffer that holds the received request
 */
Request::Request(size_t socket_fd, char *buffer)
{
	_socket_fd = socket_fd;
	_stream << buffer;
	// if (_stream.str().empty())
	// 	throw std::invalid_argument("empty request");
}

/**
 * Request destructor
 */
Request::~Request(void)
{
}

/**
 * Request copt constructor
 *
 * @param req the request to copy from
 */
Request::Request(const Request &req) { *this = req; }

/**
 * Request assignement operator
 *
 * @param req the request to assign from
 */
Request &Request::operator=(const Request &req)
{
	(void)req;
	return (*this);
}

/**
 * Get a line ending with CRLF from the request
 *
 * @param line a reference to the string to put the line in
 * @return returns a boolean
 */
bool Request::_readline(std::string &line)
{
	size_t start_i = _stream.tellg();
	size_t end_i;

	if (_stream.peek() == -1)
		return (false);
	end_i = _stream.str().find("\r\n", start_i);
	if (end_i != std::string::npos)
	{
		line = _stream.str().substr(start_i, end_i - start_i);
		_stream.seekg(end_i + 2);
		return (true);
	}
	line = _stream.str().substr(start_i);
	_stream.seekg(0, _stream.end);
	return (false);
}

void Request::_setRequestLine(std::string &line)
{
	// TODO: handle errors -> empty line, line format "METHOD PATH VERSION",
	utils::t_str_arr tokens;

	tokens = utils::split_str(line, ' ');
	_method = tokens[0];
	_path = tokens[1];
	_version = tokens[2];
	tokens.clear();
}

void Request::_setHeader(std::string &line)
{
	utils::t_str_arr header;

	header = utils::split_str(line, ": ");
	if (header.size() == 2)
		_headers[header[0]] = header[1];
}

/**
 * parse the request buffer into the Request
 *
 * @param	server_config a Config class instance containing the server config
 */
void Request::parseRequest(Config &server_config)
{
	std::string temp_line;

	// Set Request line
	while (_readline(temp_line))
		if (!temp_line.empty())
			break;
	_setRequestLine(temp_line);

	// Set Request headers
	while (_readline(temp_line))
	{
		if (temp_line.empty())
			break;
		_setHeader(temp_line);
	}

	if (_headers.count("Host") > 0)
	{
		utils::t_str_arr split_host = utils::split_str(_headers["Host"], ':');
		if (split_host.size() == 2)
		{
			_domain = split_host[0];
			_port = atoi(split_host[1].c_str());
		}
	}

	// Set Request body
	while (_readline(temp_line))
		_body.append(temp_line);

	// _config = server_config.get_config(*this);

	std::cout << *this << std::endl;
}

/**
 * Check the parsed request for errors
 */
void Request::_checkRequest(void) {}

/**
 * Output stream insertion operator overload
 *
 * @param	stream a reference to the an output stream
 * @param	request a Request class instance to output
 * @return	returns a reference to the given output stream for next insertions
 */
std::ostream &operator<<(std::ostream &stream, const Request &request)
{
	stream << "Request from: " << request.getAddress() << ", "
		   << "on port: " << request.getPort() << ", "
		   << "domain name: " << request.getDomain() << "\n";

	stream << "Method: [" + request.getMethod() + "], "
		   << "Path: [" + request.getPath() + "], "
		   << "Version: [" + request.getVersion() + "]\n";

	Request::string_map headers = request.getHeaders();
	Request::string_map::const_iterator header_it = headers.begin();
	while (header_it != headers.end())
	{
		stream << "\t[" + header_it->first + "]: [" + header_it->second + "]\n";
		header_it++;
	}

	stream << "Config:\n";
	Request::string_map config = request.getConfig();
	Request::string_map::const_iterator config_it = config.begin();
	while (config_it != config.end())
	{
		stream << "\t[" + config_it->first + "]: [" + config_it->second + "]\n";
		config_it++;
	}

	if (request.getBody().size() > 0)
		std::cout << "[Body]: \n"
				  << request.getBody() << std::endl;

	return (stream);
}
