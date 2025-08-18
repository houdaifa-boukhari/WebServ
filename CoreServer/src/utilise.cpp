/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 11:00:39 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/18 17:19:54 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool chunkedRequest(const std::string &request)
{
    size_t pos, body_start, body_end;

    pos = request.find("Transfer-Encoding: chunked");
    if (pos != std::string::npos)
    {
        std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] "
                  << "Chunked request detected" << WHIET << std::endl;
        // Find the start of the body (\r\n\r\n)
        body_start = request.find("\r\n\r\n");
        if (body_start == std::string::npos) // Check if the body delimiter exists
        {
            std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
                      << "Invalid chunked request format" << WHIET << std::endl;
            return (false);
        }
        body_start += 4; // Skip past the \r\n\r\n
        body_end = request.find("\r\n0\r\n\r\n", body_start);
        if (body_end == std::string::npos) // Check if the end of the body exists
            return (false);
        std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] "
                  << "Chunked request processing complete" << WHIET << std::endl;
        return (true);
    }
    return (false);
}

bool Connection::RequestIsComplete(const std::vector<char> &request)
{
    size_t pos, start, end, len, body_start, body_size;
    std::string header, contentLengthStr;

    pos = std::search(request.begin(), request.end(), "\r\n\r\n", "\r\n\r\n" + 4) - request.begin();
    if (pos >= request.size())
    {
        std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] "
                  << "Headers not complete yet" << WHIET << std::endl;
        return (false);
    }
    body_start = pos + 4;
    size_t line_end = std::search(request.begin(), request.end(), "\r\n", "\r\n" + 2) - request.begin();
    if (line_end >= request.size())
        return (false);
    header = std::string(request.begin(), request.begin() + line_end);
    if (header.find("GET") == 0 || header.find("DELETE") == 0)
        return (true);
    else if (header.find("POST") == 0)
    {
        if (std::search(request.begin(), request.end(), "Transfer-Encoding: chunked",
                        "Transfer-Encoding: chunked" + 25) != request.end())
            return (chunkedRequest(std::string(request.begin(), request.end())));
        pos = std::search(request.begin(), request.end(), "Content-Length:", "Content-Length:" + 15) - request.begin();
        if (pos >= request.size())
            return (false);

        start = pos + 15;
        while (start < request.size() && (request[start] == ' ' || request[start] == '\t'))
            start++;
        end = std::search(request.begin() + start, request.end(), "\r\n", "\r\n" + 2) - request.begin();
        if (end >= request.size())
            return (false);
        contentLengthStr = std::string(request.begin() + start, request.begin() + end);
        len = ::atoi(contentLengthStr.c_str());
        std::cout << "len is " << len << "\n";
        if (len > this->config.getMaxBodySizeBytes())
        {
            std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
                      << "Request is larger than defined max body size  " << WHIET << std::endl;
            NewResponse::sendSimpleErrorResponse(Client_fd, 413, config);
            return (false);
        }
        body_size = request.size() - body_start;
        std::cout << "Request size: " << request.size() << std::endl;
        std::cout << "Body start: " << body_start << std::endl;
        std::cout << "Calculated body size: " << body_size << std::endl;
        if (body_size < len)
            return (false);
        else if (body_size > len)
        {
            NewResponse::sendSimpleErrorResponse(Client_fd, 413, config);
            std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
                      << "Body size is larger than Content-Length From Client" << WHIET << std::endl;
            setNeedClose(true);
            return (false);
        }
        return (true);
    }
    else
    {
        NewResponse::sendSimpleErrorResponse(Client_fd, 405, config);
        std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
                  << "Unknown method From Client" << WHIET << std::endl;
    }
    return (false);
}

void Server::checkTimeouts()
{
	time_t now = time(NULL);

	std::map<int, Connection>::iterator it = _connections.begin();
	while (it != _connections.end())
	{
		if (now - it->second.getLastActivity() > _timeoutSec)
		{
			std::cout << YELLOW << currentTime() << MAGENTA << " [TIMEOUT] Closing " << it->first
					  << " after " << _timeoutSec << "s inactivity" << WHIET << std::endl;
			closeConnection((it++)->first);
		}
		else
			++it;
	}
}
