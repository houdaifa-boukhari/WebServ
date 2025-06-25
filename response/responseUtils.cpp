/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:48:49 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/25 12:04:06 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseUtils.hpp"

responseUtils::responseUtils()
{
}

responseUtils::~responseUtils()
{
}

std::string responseUtils::toLowerCase(const std::string &original)
{
	std::string result = original;
	std::transform(result.begin(), result.end(), result.begin(),
				   static_cast<int (*)(int)>(std::tolower));
	return result;
}