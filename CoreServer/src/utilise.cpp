/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 11:00:39 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/07/31 12:37:15 by yel-moun         ###   ########.fr       */
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
		// Find the start of the body
		body_start = request.find("\r\n\r\n");
		if (body_start == std::string::npos)
		{
			std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
					  << "Invalid chunked request format" << WHIET << std::endl;
			return false;
		}
		body_start += 4; // Skip past the \r\n\r\n
		body_end = request.find("\r\n0\r\n\r\n", body_start);
		if (body_end == std::string::npos)
			return (false);
		std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] "
				  << "Chunked request processing complete" << WHIET << std::endl;
		return (true);
	}
	return (false);
}

bool Connection::RequestIsComplete(const std::string &request)
{
	size_t pos, start, end, len, body_start, body_size;
	std::string header, contentLengthStr;

	pos = request.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] "
				  << "Headers not complete yet" << WHIET << std::endl;
		return (false);
	}
	// Get first line (request line)
	size_t line_end = request.find("\r\n");
	if (line_end == std::string::npos)
		return (false);
	header = request.substr(0, line_end);

	if (header.find("GET") == 0 || header.find("DELETE") == 0)
	{
		return (true);
	}
	else if (header.find("POST") == 0)
	{
		if (request.find("Transfer-Encoding: chunked") != std::string::npos)
			return (chunkedRequest(request));
		pos = request.find("Content-Length:");
		if (pos == std::string::npos)
			return (false);
		start = pos + 15;
		while (start < request.size() && (request[start] == ' ' || request[start] == '\t'))
			start++;
		end = request.find("\r\n", start);
		if (end == std::string::npos)
			return (false);
		contentLengthStr = request.substr(start, end - start);
		len = ::atoi(contentLengthStr.c_str());
		std::cout << "len is " << len << "\n";
		if (len > this->config.getMaxBodySizeBytes())
		{
			std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
					  << "Request is larger than defined max body size  " << WHIET << std::endl;
			NewResponse::sendSimpleErrorResponse(Client_fd, 413, config);
			return false;
		}
		// Body starts right after \r\n\r\n
		body_start = request.find("\r\n\r\n") + 4;
		body_size = request.size() - body_start;
		if (body_size < len)
			return (false);
		else if (body_size > len)
		{
			NewResponse::sendSimpleErrorResponse(Client_fd, 413, config);
			std::cerr
				<< YELLOW << currentTime() << RED << " [ERROR] "
				<< "Body size is larger than Content-Length From Client" << WHIET << std::endl;
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
