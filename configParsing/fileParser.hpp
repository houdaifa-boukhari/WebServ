/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:29:26 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/09 17:33:09 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include <iostream>
#include <fstream>
#include "parsingUtils.hpp"
#include "config.hpp"

#define FILE_EXTENSION ".conf"
class FileParser
{
private:
	std::string _filename;
	std::ifstream _configFile;
	std::vector<std::string> _configLines;
	Config _config;
	bool checkFileExtension();
	bool checkFileExistence();
	void readConfigFile();
	bool checkBrackets();
	bool checkEndOfLine();
	int parseServerBlock(ServerConfig &server, int blockStart);
	int parseLocationBlock(LocationConfig &location, int blockStart);

public:
	FileParser(int argc, char **argv);
	void start();
	void parseFile();
	Config getConfig();
	class WrongExtentionException : public std::exception
	{
		virtual const char *what() const throw();
	};
	class WrongFileFormatException : public std::exception
	{
		virtual const char *what() const throw();
	};
	class WrongFileException : public std::exception
	{
		virtual const char *what() const throw();
	};
	class FileDoesNotExistException : public std::exception
	{
		virtual const char *what() const throw();
	};
};
#endif