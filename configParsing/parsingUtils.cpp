/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:47:33 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/02 22:25:44 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsingUtils.hpp"

ParsingUtils::ParsingUtils() {}

ParsingUtils::~ParsingUtils() {}

std::string ParsingUtils::removeWhiteSpaces(std::string const &str)
{
	std::string::size_type start = str.find_first_not_of(" \t");
	std::string::size_type end = str.find_last_not_of(" \t");

	if (start == std::string::npos || end == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

bool ParsingUtils::isComment(std::string const &str)
{
	std::string::size_type start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
		return false;
	return str[start] == '#';
}

std::string ParsingUtils::cleanInLineComment(std::string const &str)
{
	std::string::size_type start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
		return "";
	std::string::size_type end = str.find("#");
	if (end == std::string::npos)
		return str;
	return str.substr(start, end - start);
}

std::string ParsingUtils::getKey(std::string const &str)
{
	std::string::size_type end = str.find(" ");
	if (end == std::string::npos)
		return str;
	return str.substr(0, end);
}

std::string ParsingUtils::getValue(std::string const &str)
{
	std::string::size_type start = str.find(" ");
	if (start == std::string::npos)
		return "";
	std::string::size_type end = str.find(";");
	if (end == std::string::npos)
		return "";
	return str.substr(start + 1, end - start - 1);
}

double ParsingUtils::convertToNumber(std::string const &str)
{
	char *endptr;
	double value = std::strtod(str.c_str(), &endptr);

	if (*endptr != '\0')
		throw FileParser::WrongFileContentException();

	return value;
}

std::vector<std::string> ParsingUtils::splitString(std::string const &str, char delim)
{
	std::vector<std::string> tokens;
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	while ((end = str.find(delim, start)) != std::string::npos)
	{
		std::string token = str.substr(start, end - start);
		token = removeWhiteSpaces(token);
		if (!token.empty())
			tokens.push_back(token);
		start = end + 1;
	}

	std::string lastToken = str.substr(start);
	lastToken = removeWhiteSpaces(lastToken);
	if (!lastToken.empty())
		tokens.push_back(lastToken);

	return tokens;
}