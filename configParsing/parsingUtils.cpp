/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:47:33 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/10 21:50:48 by yel-moun         ###   ########.fr       */
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