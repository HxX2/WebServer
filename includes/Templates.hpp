/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Templates.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 10:29:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 05:01:18 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _Templates_HPP_
#define _Templates_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <vector>
#include <utils.hpp>

class Templates
{
private:
	DIR *_dir;
	struct dirent *_ent;
	std::string _path;
	std::vector<char> _buffer;
	std::ifstream _htmlFile;
	std::string _links;
	std::string _html;

public:
	Templates();
	~Templates();

	void index(const std::string &path);
	void setLinks(std::string filePath, std::string fileName);
	std::string getIndexerPage();
	std::string getErrorPage(std::string status, std::string path);
};

#endif