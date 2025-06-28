/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:49:04 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/28 15:43:22 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEUTILS_HPP
#define RESPONSEUTILS_HPP

#include <iostream>
#include <algorithm>
#include <map>

class responseUtils
{
private:
	responseUtils(/* args */);
	~responseUtils();

public:
	static std::string toLowerCase(const std::string &original);
	static std::map<std::string, size_t> extractRangeHeaders(const std::string &rawHeader);
	static std::string trimString(const std::string &str);
	static std::string getCurrentDate();
};

#endif