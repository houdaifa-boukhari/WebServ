/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:19:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/05/09 16:42:38 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

bool Server::RequestIsComplete(const std::string& request)
{
	size_t pos, start, end, len, body_start, body_size;
	std::string header, contentLengthStr;

	// Check if headers are complete
	pos = request.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		std::cerr << currentTime() << " [ERROR] " << "Headers not complete yet" << std::endl;
		return (false);
	}
	// Get first line (request line)
	size_t line_end = request.find("\r\n");
	if (line_end == std::string::npos)
		return (false);
	header = request.substr(0, line_end);

	if (header.find("GET") == 0 || header.find("DELETE") == 0)
		return (true);
	if (header.find("POST") == 0)
	{
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
		// Body starts right after \r\n\r\n
		body_start = request.find("\r\n\r\n") + 4;
		body_size = request.size() - body_start;
		std::string name = "numbers.txt";
    	std::ofstream ofs;
		
		ofs.open (name.c_str());
		ofs << "Content lenght : " << len << ", " << "request_size : " << request.size() 
			<< ", body start : "<< body_start << std::endl;


		
		if (body_size < len)
			return (false);
		return (true);
	}
	return (false);
}

void Server::generateResponse(int ClientFd)
{
	std::cout << currentTime() << " [INFO] " << "Generating response for client " << ClientFd << std::endl;
	std::string    exampleHtml = "<html><body><h1> <center> Welcome to 1337 | testing Webserv </center></h1></body></html>";
    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: " + std::to_string(exampleHtml.size()) + "\r\n"
                           "Access-Control-Allow-Origin: *\r\n" 
                           "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n" 
                           "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                           "\r\n" + exampleHtml;
	if (send(ClientFd, response.c_str(), response.size(), 0) < 0)
	{
		std::cerr << currentTime() << " [ERROR] " << "Failed to send response to client " << ClientFd << std::endl;
		closeConnection(ClientFd);
		return ;
	}
	std::cout << currentTime() << " [DEBUG] " << "Response sent to client " << ClientFd << std::endl;
}

void Server::checkTimeouts()
{
    time_t now = time(NULL);

    std::map<int, time_t>::iterator it = _lastActivity.begin();
    while (it != _lastActivity.end())
	{
        if (now - it->second > _timeoutSec)
		{
            std::cout << currentTime() << " [TIMEOUT] Closing " << it->first
                      << " after " << _timeoutSec << "s inactivity\n";
            closeConnection((it++)->first);
        }
		else
        	++it;
    }
}
