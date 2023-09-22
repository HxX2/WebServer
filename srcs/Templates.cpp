/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Templates.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 10:37:30 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 06:03:02 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Templates.hpp>

Templates::Templates()
{
}

Templates::~Templates()
{
	_htmlFile.close();
}

void Templates::index(const std::string &path, const std::string &location)
{
	_location = location[location.length() - 1] == '/' ? location : location + "/";
	_dir = opendir(path.c_str());
	if (_dir == NULL)
		utils::log("ERROR", "Indexer: Cannot open directory");
	while ((_ent = readdir(_dir)) != NULL)
	{
		this->setLinks(path + std::string("/") + std::string(_ent->d_name), std::string(_ent->d_name));
	}
	closedir(_dir);
}

void Templates::setLinks(std::string filePath, std::string fileName)
{
	struct stat fileStat;
	if (stat(filePath.c_str(), &fileStat) == -1)
		utils::log("ERROR", "Indexer: Cannot get file info");
	std::time_t ctime = fileStat.st_ctime;
	std::tm *ptm = std::localtime(&ctime);
	char ftime[32];
	std::strftime(ftime, sizeof(ftime), "%d-%m-%Y %H:%M", ptm);

	if (fileName != "." && fileName != "..")
	{
		if (S_ISDIR(fileStat.st_mode))
			_links += "<a href=\"" + _location + fileName + "/\">" + fileName + "</a><p>" + std::string(ftime) + "</p><span>" + utils::to_string(fileStat.st_size / 1024) + " KB</span>";
		else
			_links += "<a href=\"" + _location + fileName + "\">" + fileName + "</a><p>" + std::string(ftime) + "</p><span>" + utils::to_string(fileStat.st_size / 1024) + " KB</span>";
	}
}

std::string Templates::getIndexerPage()
{
	_htmlFile.open("./templates/Indexer.html");

	if (_htmlFile.is_open())
	{
		_buffer.assign(std::istreambuf_iterator<char>(_htmlFile), std::istreambuf_iterator<char>());
		_html = std::string(_buffer.begin(), _buffer.end());

		_html.replace(_html.find("{{links}}"), 9, _links);
		_html.replace(_html.find("{{location}}"), 12, _location);
		return (_html);
	}
	else
		utils::log("ERROR", "Indexer: Cannot open html file");
	return ("");
}

std::string Templates::getErrorPage(std::string status, std::string path)
{
	path == "" ? _htmlFile.open("./templates/Error.html") : _htmlFile.open(path.c_str());
	std::string http_msg = utils::http_msg(status);

	if (_htmlFile.is_open())
	{
		_buffer.assign(std::istreambuf_iterator<char>(_htmlFile), std::istreambuf_iterator<char>());
		_html = std::string(_buffer.begin(), _buffer.end());

		if (path == "")
		{
			_html.replace(_html.find("{{status}}"), 10, status);
			_html.replace(_html.find("{{http_msg}}"), 12, http_msg);
		}
		return (_html);
	}
	else
		utils::log("WARNING", "Indexer: Cannot open html file");
	return ("");
}
