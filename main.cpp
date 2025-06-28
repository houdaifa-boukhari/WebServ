/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:47:22 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/28 11:29:12 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParsing/fileParser.hpp"
#include "./CoreServer/include/Server.hpp"

std::string toLower(const std::string &str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

std::map<std::string, size_t> extractRangeHeaders(const std::string &rawHeader)
{
	std::map<std::string, size_t> rangeHeaders;
	const size_t MAX_CHUNK_SIZE = 2 * 1024 * 1024;

	size_t start = 0;
	size_t end = MAX_CHUNK_SIZE;

	if (toLower(rawHeader.substr(0, 6)) == "bytes=")
	{
		const std::string range = rawHeader.substr(6);
		size_t dashPos = range.find('-');
		if (dashPos != std::string::npos)
		{
			std::string startStr = trim(range.substr(0, dashPos));
			std::string endStr = trim(range.substr(dashPos + 1));

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
			}
			if (end < start)
			{
				start = 0;
				end = MAX_CHUNK_SIZE;
			}
		}
	}

	rangeHeaders["start"] = start;
	rangeHeaders["end"] = end;
	return rangeHeaders;
}

int main(int argc, char *argv[])
{

	try
	{
		FileParser *fp = new FileParser(argc, argv);
		fp->start();
		fp->getServerConfiguration().printConfig();
		Server sv(fp->getServerConfiguration().getServers());
		sv.run();
		delete fp;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		std::exit(1);
	}
	catch (const char *e)
	{
		std::cerr << e << std::endl;
		std::exit(1);
	}
	return 0;
}