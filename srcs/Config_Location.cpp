#include "Config.hpp"

// ========== LocationBlock Class

LocationBlock::LocationBlock(void) : _path("") {}

LocationBlock::~LocationBlock(void) {}

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
	size_t path_start, path_end;

	path_start = line.find_first_of(" \t");
	path_end = line.find_last_of(" \t");
	_path = line.substr(path_start + 1, path_end - path_start - 1);
}

const t_directives &LocationBlock::get_directives(void) const
{
	return (_directives);
}

void LocationBlock::add_directive(std::string &line)
{
	// TODO: handle error_pages in server with inexistant location
	t_directive directive;

	directive.parse(line);
	if (!directive.is_valid())
		throw std::invalid_argument("location in config contains invalid directive");
	if (!_directives.count(directive.key))
		_directives[directive.key] = directive.value;
}

std::ostream &operator<<(std::ostream &stream, const LocationBlock &location)
{
	t_directives::const_iterator it = location.get_directives().begin();

	stream << BLUE << "\tLocation: " << RESET << "\"" << location.get_path() << "\"" << std::endl;
	while (it != location.get_directives().end())
	{
		stream << YELLOW << "\t\tDirective [ " + it->first + " ]" << RESET << " => " << it->second << std::endl;
		it++;
	}
	return (stream);
}
