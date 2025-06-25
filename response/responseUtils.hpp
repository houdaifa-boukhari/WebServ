/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:49:04 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/25 12:04:21 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEUTILS_HPP
#define RESPONSEUTILS_HPP

#include <iostream>
#include <algorithm>

class responseUtils
{
private:
	responseUtils(/* args */);
	~responseUtils();

public:
	static std::string toLowerCase(const std::string &original);
};

#endif