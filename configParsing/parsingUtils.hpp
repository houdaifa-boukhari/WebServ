/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:46:00 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/10 21:50:03 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGUTILS_HPP
#define PARSINGUTILS_HPP

#include <iostream>

class ParsingUtils
{
private:
	ParsingUtils();
	~ParsingUtils();

public:
	static std::string removeWhiteSpaces(std::string const &str);
	static bool isComment(std::string const &str);
	static std::string cleanInLineComment(std::string const &str);
	static std::string getKey(std::string const &str);
	static std::string getValue(std::string const &str);
};

#endif