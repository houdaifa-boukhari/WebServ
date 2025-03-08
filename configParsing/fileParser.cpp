/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:56:59 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/08 16:04:55 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileParser.hpp"

FileParser::FileParser(int argc, char **argv)
{
	if (argc != 2)
		throw WrongFileException();
	_filename = argv[1];
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
	std::cout << "Parsing file" << std::endl;
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