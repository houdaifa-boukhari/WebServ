/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:56:59 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/08 15:51:27 by yel-moun         ###   ########.fr       */
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
	if (_filename.find(FILE_EXTENSION, dotPos) == std::string::npos)
		return false;
	return true;
}

void FileParser::start()
{
	if (!this->checkFileExtension())
		throw WrongExtentionException();
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