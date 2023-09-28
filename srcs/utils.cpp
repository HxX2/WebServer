#include "utils.hpp"

void utils::remove_comments(std::string &str)
{
	size_t s = str.find('#');
	if (s != std::string::npos)
		str.erase(s);
}

void utils::trim_str(std::string &str)
{
	size_t s, e;

	if (str.empty())
		return;
	if (!str.empty() && str.at(str.size() - 1) == ';')
		str.erase(str.size() - 1);
	for (s = 0; str[s] == ' ' || str[s] == '\t'; s++)
		;
	str.erase(0, s);
	for (e = str.size() - 1; !str.empty() && (str[e] == ' ' || str[e] == '\t'); e--)
		;
	str.erase(e + 1);

	if ((s = str.find_first_of(" \t")) != std::string::npos)
	{
		e = str.find_first_not_of(" \t", s);
		str = str.substr(0, s) + " " + str.substr(e);
	}
}

void utils::to_lowercase(std::string &line)
{
	for (size_t i = 0; i < line.size(); i++)
		line[i] = tolower(line[i]);
}

utils::t_str_arr utils::split_str(std::string str, char sep)
{
	utils::t_str_arr result;
	size_t start = 0, sep_pos = -1;

	while ((sep_pos = str.find(sep, start)))
	{
		result.push_back(str.substr(start, sep_pos - start));
		start = sep_pos + 1;
		while (str[start] == sep)
			start++;
		if (sep_pos == std::string::npos)
			break;
	}
	return (result);
}

utils::t_str_arr utils::split_str(std::string str, std::string sep)
{
	utils::t_str_arr result;
	size_t start = 0, sep_pos = -1;

	while ((sep_pos = str.find(sep, start)))
	{
		result.push_back(str.substr(start, sep_pos - start));
		start = sep_pos + 1;
		while (sep.find(str[start]) != std::string::npos)
			start++;
		if (sep_pos == std::string::npos)
			break;
	}
	return (result);
}

size_t utils::find_in(std::string &line, const char *chars, size_t pos, const char *error)
{
	size_t i;

	if ((i = line.find_first_of(chars, pos)) == std::string::npos)
		throw std::invalid_argument(error);
	return (i);
}

size_t utils::not_find_in(std::string &line, const char *chars, size_t pos, const char *error)
{
	size_t i;

	if ((i = line.find_first_not_of(chars, pos)) == std::string::npos)
		throw std::invalid_argument(error);
	return (i);
}

bool utils::vector_contains(utils::t_str_arr &array, const std::string &element)
{
	for (size_t i = 0; i < array.size(); i++)
	{
		if (array[i] == element)
			return (true);
	}
	return (false);
}

std::string utils::to_string(long value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string utils::http_msg(std::string status)
{
	std::map<std::string, std::string> statusMessages;

	statusMessages["100"] = "Continue";
	statusMessages["101"] = "Switching Protocols";
	statusMessages["200"] = "OK";
	statusMessages["201"] = "Created";
	statusMessages["202"] = "Accepted";
	statusMessages["203"] = "Non-Authoritative Information";
	statusMessages["204"] = "No Content";
	statusMessages["205"] = "Reset Content";
	statusMessages["206"] = "Partial Content";
	statusMessages["300"] = "Multiple Choices";
	statusMessages["301"] = "Moved Permanently";
	statusMessages["302"] = "Found";
	statusMessages["303"] = "See Other";
	statusMessages["304"] = "Not Modified";
	statusMessages["307"] = "Temporary Redirect";
	statusMessages["308"] = "Permanent Redirect";
	statusMessages["400"] = "Bad Request";
	statusMessages["403"] = "Forbidden";
	statusMessages["404"] = "Not Found";
	statusMessages["405"] = "Method Not Allowed";
	statusMessages["409"] = "Conflict";
	statusMessages["411"] = "Length Required";
	statusMessages["413"] = "Payload Too Large";
	statusMessages["500"] = "Internal Server Error";
	statusMessages["505"] = "HTTP Version Not Supported";

	return (statusMessages[status]);
}

std::string utils::mimetypes(std::string value, bool reverse)
{
	std::map<std::string, std::string> mimeTypes;

	mimeTypes["html"] = "text/html";
	mimeTypes["htm"] = "text/html";
	mimeTypes["shtml"] = "text/html";
	mimeTypes["css"] = "text/css";
	mimeTypes["xml"] = "text/xml";
	mimeTypes["rss"] = "text/xml";
	mimeTypes["gif"] = "image/gif";
	mimeTypes["jpeg"] = "image/jpeg";
	mimeTypes["jpg"] = "image/jpeg";
	mimeTypes["js"] = "application/x-javascript";
	mimeTypes["json"] = "application/json";
	mimeTypes["txt"] = "text/plain";
	mimeTypes["htc"] = "text/x-component";
	mimeTypes["mml"] = "text/mathml";
	mimeTypes["png"] = "image/png";
	mimeTypes["ico"] = "image/x-icon";
	mimeTypes["jng"] = "image/x-jng";
	mimeTypes["wbmp"] = "image/vnd.wap.wbmp";
	mimeTypes["jar"] = "application/java-archive";
	mimeTypes["war"] = "application/java-archive";
	mimeTypes["ear"] = "application/java-archive";
	mimeTypes["hqx"] = "application/mac-binhex40";
	mimeTypes["pdf"] = "application/pdf";
	mimeTypes["cco"] = "application/x-cocoa";
	mimeTypes["jardiff"] = "application/x-java-archive-diff";
	mimeTypes["jnlp"] = "application/x-java-jnlp-file";
	mimeTypes["run"] = "application/x-makeself";
	mimeTypes["pl"] = "application/x-perl";
	mimeTypes["pm"] = "application/x-perl";
	mimeTypes["prc"] = "application/x-pilot";
	mimeTypes["pdb"] = "application/x-pilot";
	mimeTypes["rar"] = "application/x-rar-compressed";
	mimeTypes["rpm"] = "application/x-redhat-package-manager";
	mimeTypes["sea"] = "application/x-sea";
	mimeTypes["swf"] = "application/x-shockwave-flash";
	mimeTypes["sit"] = "application/x-stuffit";
	mimeTypes["tcl"] = "application/x-tcl";
	mimeTypes["tk"] = "application/x-tcl";
	mimeTypes["der"] = "application/x-x509-ca-cert";
	mimeTypes["pem"] = "application/x-x509-ca-cert";
	mimeTypes["crt"] = "application/x-x509-ca-cert";
	mimeTypes["xpi"] = "application/x-xpinstall";
	mimeTypes["zip"] = "application/zip";
	mimeTypes["iso"] = "application/octet-stream";
	mimeTypes["deb"] = "application/octet-stream";
	mimeTypes["bin"] = "application/octet-stream";
	mimeTypes["exe"] = "application/octet-stream";
	mimeTypes["dll"] = "application/octet-stream";
	mimeTypes["dmg"] = "application/octet-stream";
	mimeTypes["eot"] = "application/octet-stream";
	mimeTypes["img"] = "application/octet-stream";
	mimeTypes["msi"] = "application/octet-stream";
	mimeTypes["msp"] = "application/octet-stream";
	mimeTypes["msm"] = "application/octet-stream";
	mimeTypes["mp3"] = "audio/mpeg";
	mimeTypes["ra"] = "audio/x-realaudio";
	mimeTypes["mpeg"] = "video/mpeg";
	mimeTypes["mpg"] = "video/mpeg";
	mimeTypes["mov"] = "video/quicktime";
	mimeTypes["flv"] = "video/x-flv";
	mimeTypes["avi"] = "video/x-msvideo";
	mimeTypes["wmv"] = "video/x-ms-wmv";
	mimeTypes["asx"] = "video/x-ms-asf";
	mimeTypes["asf"] = "video/x-ms-asf";
	mimeTypes["mng"] = "video/x-mng";

	if (reverse)
	{
		for (std::map<std::string, std::string>::iterator it = mimeTypes.begin(); it != mimeTypes.end(); it++)
		{
			if (it->second == value)
				return ("." + it->first);
		}
		return ("");
	}
	else
		return (mimeTypes[value]);
}

std::string utils::http_date()
{
	std::time_t t = std::time(0);
	std::tm *now = std::gmtime(&t);

	const char *format = "%a, %d %b %Y %H:%M:%S GMT";
	char buffer[64];

	std::strftime(buffer, sizeof(buffer), format, now);

	return (std::string(buffer));
}

void utils::log(std::string type, std::string msg)
{
	if (type == "DEBUG")
		std::cout << GREEN << "[" << type << "] " << RESET << msg << std::endl;
	else if (type == "ERROR")
		std::cerr << RED << "[" << type << "] " << RESET << msg << std::endl;
	else if (type == "WARNING")
		std::cerr << YELLOW << "[" << type << "] " << RESET << msg << std::endl;
	else if (type == "INFO")
		std::cout << MAGENTA << "[" << type << "] " << RESET << msg << std::endl;
}

void utils::time_now(std::string &nstr)
{
	time_t now = std::time(0);

	while (now > 0)
	{
		nstr.insert(0, 1, now % 10 + '0');
		now /= 10;
	}
}

unsigned long utils::hex_to_decimal(const std::string &hex)
{
	unsigned long dec;
	std::stringstream ss;

	ss << std::hex << hex;
	ss >> dec;
	return (dec);
}

bool utils::is_dir(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return (true);
	}
	return (false);
}