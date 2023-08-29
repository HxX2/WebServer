/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Indexer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 10:37:30 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 06:03:02 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Modules.hpp>

Indexer::Indexer()
{
}

Indexer::~Indexer()
{
}

void Indexer::index(const std::string &path)
{
	_dir = opendir(path.c_str());
	if (_dir == NULL)
		this->log("ERROR", "Indexer : Cannot open directory");
	while ((_ent = readdir(_dir)) != NULL)
	{
		this->setLinks(path + std::string("/") + std::string(_ent->d_name), std::string(_ent->d_name));
	}
	closedir(_dir);
}

void Indexer::setLinks(std::string filePath, std::string fileName)
{
	struct stat fileStat;
	if (stat(filePath.c_str(), &fileStat) == -1)
		this->log("ERROR", "Indexer : Cannot get file info");
	std::time_t ctime = fileStat.st_ctime;
	std::tm *ptm = std::localtime(&ctime);
	char ftime[32];
	std::strftime(ftime, sizeof(ftime), "%d-%m-%Y %H:%M", ptm);

	if (fileName != "." && fileName != "..")
	{
		if (S_ISDIR(fileStat.st_mode))
			_links += "<a href=\"" + fileName + "/\">" + fileName + "</a><p>" + std::string(ftime) + "</p><span>" + utils::to_string(fileStat.st_size / 1024) + " KB</span>";
		else
			_links += "<a href=\"" + fileName + "\">" + fileName + "</a><p>" + std::string(ftime) + "</p><span>" + utils::to_string(fileStat.st_size / 1024) + " KB</span>";
	}
}

std::string Indexer::getHtml()
{
	_htmlFile.open("/root/LAB/WebServer/html/Indexer.html");

	if (_htmlFile.is_open())
	{
		_buffer.assign(std::istreambuf_iterator<char>(_htmlFile), std::istreambuf_iterator<char>());
		_html = std::string(_buffer.begin(), _buffer.end());
		_htmlFile.close();

		_html.replace(_html.find("{{links}}"), 9, _links);
		_html.replace(_html.find("{{location}}"), 12, "/");
		return (_html);
	}
	else
		this->log("ERROR", "Indexer : Cannot open html file");
	return ("");
}

void Indexer::close()
{
}
