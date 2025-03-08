/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:29:26 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/08 15:46:01 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include <iostream>
#define FILE_EXTENSION ".conf"
class FileParser
{
private:
	std::string _filename;

public:
	FileParser(int argc, char **argv);
	void start();
	void parseFile();
	bool checkFileExtension();
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
};
#endif