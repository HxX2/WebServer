#include "Config.hpp"

// ========== LocationBlock Class

LocationBlock::LocationBlock(void) {}

LocationBlock::~LocationBlock(void)
{
	_directives.clear();
}

LocationBlock::LocationBlock(const LocationBlock &block) { *this = block; }

LocationBlock &LocationBlock::operator=(const LocationBlock &block)
{
	_path = block._path;
	_directives = block._directives;
	return (*this);
}

size_t LocationBlock::size(void) const
{
	return (_directives.size());
}

const std::string &LocationBlock::get_path(void) const
{
	return (_path);
}

void LocationBlock::set_path(std::string &line)
{
	// TODO: check if path is valid
	// ? line = "location /path/of/location{}"
	size_t path_start, path_end;

	path_start = line.find_first_of(" \t");
	path_end = line.find_last_of(" \t");
	_path = line.substr(path_start + 1, path_end - path_start - 1);
}

const std::map<std::string, std::string> &LocationBlock::get_directives(void) const
{
	return (_directives);
}

void LocationBlock::add_directive(std::string &line)
{
	t_directive directive;
	size_t separator_index;

	directive.parse(line);
	if (!directive.is_valid() || directive.key == "host" || directive.key == "port" || directive.key == "server_name")
		throw std::invalid_argument("location in config contains invalid directive");
	if (directive.key == "error_page" || directive.key == "cgi")
	{
		separator_index = directive.value.find(":");
		if (separator_index != std::string::npos)
		{
			directive.key.append("_" + directive.value.substr(0, separator_index));
			directive.value.erase(0, separator_index + 1);
		}
	}
	if (!_directives.count(directive.key))
		_directives[directive.key] = directive.value;
}

std::ostream &operator<<(std::ostream &stream, const LocationBlock &location)
{
	std::map<std::string, std::string>::const_iterator it = location.get_directives().begin();

	stream << BLUE << "\tLocation: " << RESET << "\"" << location.get_path() << "\"" << std::endl;
	while (it != location.get_directives().end())
	{
		stream << YELLOW << "\t\tDirective [ " + it->first + " ]" << RESET << " => " << it->second << std::endl;
		it++;
	}
	return (stream);
}
