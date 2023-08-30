/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Indexer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlafou <zlafou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 10:29:04 by zlafou            #+#    #+#             */
/*   Updated: 2023/08/26 05:01:18 by zlafou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _INDEXER_HPP_
#define _INDEXER_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <vector>
#include <utils.hpp>

class Indexer
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
	Indexer();
	~Indexer();

	void index(const std::string &path);
	void setLinks(std::string filePath, std::string fileName);
	std::string getHtml();
	void close();
};

#endif