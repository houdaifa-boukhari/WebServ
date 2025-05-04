/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:29:26 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/02 22:22:37 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include <iostream>
#include <fstream>
#include "parsingUtils.hpp"
#include "serverConfiguration.hpp"

#define FILE_EXTENSION ".conf"
class FileParser
{
private:
	std::string _filename;
	std::ifstream _configFile;
	std::vector<std::string> _configLines;
	ServerConfiguration _serverConfiguration;
	bool checkFileExtension();
	bool checkFileExistence();
	void readConfigFile();
	bool checkBrackets();
	bool checkEndOfLine();
	std::vector<std::string>::iterator handleLocationBlock(std::vector<std::string>::iterator &it, ServerConfig &ServerConfig);
	std::vector<std::string>::iterator handleServerBlock(std::vector<std::string>::iterator &it);
	void parseServerDirective(const std::string &directive, ServerConfig &serverConfig);
	void parseLocationDirective(const std::string &directive, LocationConfig &locationConfig);

public:
	FileParser(int argc, char **argv);
	void start();
	void parseFile();
	ServerConfiguration getServerConfiguration();
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
	class WrongFileContentException : public std::exception
	{
		virtual const char *what() const throw();
	};
};
#endif