/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:56:59 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/09 17:47:16 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileParser.hpp"

FileParser::FileParser(int argc, char **argv)
{
	if (argc != 2)
		throw WrongFileException();
	_filename = argv[1];
	_config = Config();
}

bool FileParser::checkFileExtension()
{
	size_t dotPos = _filename.find_last_of(".");
	if (dotPos == std::string::npos)
		return false;
	size_t extPos = _filename.find(FILE_EXTENSION);
	if (extPos == std::string::npos)
		return false;
	if (extPos + 5 != _filename.length())
		return false;
	return true;
}

void FileParser::start()
{
	if (!this->checkFileExtension())
		throw WrongExtentionException();
	if (!this->checkFileExistence())
		throw FileDoesNotExistException();
	this->readConfigFile();
	if (!this->checkBrackets() || !this->checkEndOfLine())
		throw WrongFileFormatException();
	this->parseFile();
}

void FileParser::readConfigFile()
{
	std::string line;
	while (std::getline(this->_configFile, line))
	{
		if (line.empty())
			continue;
		if (ParsingUtils::isComment(line))
			continue;
		line = ParsingUtils::removeWhiteSpaces(line);
		line = ParsingUtils::cleanInLineComment(line);
		if (line.empty())
			continue;
		this->_configLines.push_back(line);
	}
}

bool FileParser::checkFileExistence()
{
	this->_configFile.open(_filename);
	if (!this->_configFile.good())
		return false;
	return true;
}

void FileParser::parseFile()
{
	for (size_t i = 0; i < this->_configLines.size(); i++)
	{
		std::cout << this->_configLines[i] << std::endl;
	}
}

int FileParser::parseServerBlock(ServerConfig &server, int blockStart)
{
	(void)server;
	(void)blockStart;
	return 0;
}

int FileParser::parseLocationBlock(LocationConfig &location, int blockStart)
{
	(void)location;
	(void)blockStart;
	return 0;
}

Config FileParser::getConfig()
{
	return this->_config;
}

bool FileParser::checkBrackets()
{
	int brackets = 0;

	for (size_t i = 0; i < this->_configLines.size(); i++)
	{
		if (this->_configLines[i].find("{") != std::string::npos)
			brackets++;
		if (this->_configLines[i].find("}") != std::string::npos)
			brackets--;
	}
	return brackets == 0;
}

bool FileParser::checkEndOfLine()
{
	for (size_t i = 0; i < this->_configLines.size(); i++)
	{

		if (this->_configLines[i].find("server") != std::string::npos)
		{
			if (this->_configLines[i].length() == 6)
				continue;
			else if ((this->_configLines[i].find("{") != std::string::npos) && (this->_configLines[i].find("{") + 1) == this->_configLines[i].length())
				continue;
			else
			{
				if (this->_configLines[i].find(";") != std::string::npos)
					continue;
				else
					return (false);
			}
		}
		else if (this->_configLines[i].find("location") != std::string::npos)
		{
			if (this->_configLines[i].length() == 8)
				continue;
			else if ((this->_configLines[i].find("{") != std::string::npos) && (this->_configLines[i].find("{") + 1) == this->_configLines[i].length())
				continue;
			else
			{
				if (this->_configLines[i].find(";") != std::string::npos)
					continue;
				else
					return (false);
			}
		}
		else if (this->_configLines[i].find("}") != std::string::npos)
		{
			if (this->_configLines[i].length() == 1)
				continue;
			else
				return (false);
		}
		else if (this->_configLines[i].find("{") != std::string::npos)
		{
			if (this->_configLines[i].length() == 1)
				continue;
			else
			{
				if (this->_configLines[i].find(";") != std::string::npos)
					continue;
				else
					return (false);
			}
		}
		else
		{
			if (this->_configLines[i].find(";") != std::string::npos)
				continue;
			else
				return (false);
		}
	}
	return true;
}

const char *FileParser::WrongExtentionException::what() const throw()
{
	return "Wrong file extention example: file.conf";
}

const char *FileParser::WrongFileFormatException::what() const throw()
{
	return "Wrong file format";
}

const char *FileParser::WrongFileException::what() const throw()
{
	return "Please provide a valid file as argument";
}

const char *FileParser::FileDoesNotExistException::what() const throw()
{
	return "File does not exist";
}