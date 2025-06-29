/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:48:49 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/28 15:43:39 by yel-moun         ###   ########.fr       */
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

std::string responseUtils::trimString(const std::string &str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

std::map<std::string, size_t> responseUtils::extractRangeHeaders(const std::string &rawHeader)
{
	std::map<std::string, size_t> rangeHeaders;
	const size_t MAX_CHUNK_SIZE = 100 * 1024 * 1024;

	size_t start = 0;
	size_t end = MAX_CHUNK_SIZE;

	if (responseUtils::toLowerCase(rawHeader.substr(0, 6)) == "bytes=")
	{
		const std::string range = rawHeader.substr(6);
		size_t dashPos = range.find('-');
		if (dashPos != std::string::npos)
		{
			std::string startStr = responseUtils::trimString(range.substr(0, dashPos));
			std::string endStr = responseUtils::trimString(range.substr(dashPos + 1));

			try
			{
				if (!startStr.empty())
					start = std::stoull(startStr);
				if (!endStr.empty())
					end = std::stoull(endStr);
			}
			catch (const std::exception &e)
			{
				start = 0;
				end = MAX_CHUNK_SIZE;
				rangeHeaders["error"] = std::string::npos;
			}
			if (end < start)
			{
				start = 0;
				end = MAX_CHUNK_SIZE;
				rangeHeaders["error"] = std::string::npos;
			}
		}
	}
	rangeHeaders["start"] = start;
	rangeHeaders["end"] = end;
	return rangeHeaders;
}

std::string responseUtils::getCurrentDate()
{
	time_t now = time(0);
	struct tm *tm = gmtime(&now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buffer);
}