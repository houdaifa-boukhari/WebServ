/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilise.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:19:09 by hel-bouk          #+#    #+#             */
<<<<<<< HEAD:CoreServer/src/server/utilise.cpp
/*   Updated: 2025/05/09 16:42:38 by hel-bouk         ###   ########.fr       */
=======
/*   Updated: 2025/06/28 18:12:34 by hel-bouk         ###   ########.fr       */
>>>>>>> 7a95435823a8143339d8c12718799f4ad8aa7c87:CoreServer/src/utilise.cpp
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

bool Server::RequestIsComplete(const std::string& request)
{
	size_t pos, start, end, len, body_start, body_size;
	std::string header, contentLengthStr;

	// Check if headers are complete
	pos = request.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Headers not complete yet" << WHIET << std::endl;
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
		else if (body_size > len)
		{
			// Errror: Body size is larger than Content-Length (youssef)
			std::cerr << YELLOW << currentTime() << RED << " [ERROR] " << "Body size is larger than Content-Length From Client" << WHIET << std::endl;
			return (false);
		}
		return (true);
	}
	// Error Unknown method
	std::cerr << YELLOW << currentTime() << RED << " [ERROR] " << "Unknown method From Client" << WHIET << std::endl;
	return (false);
}

void Server::generateResponse(int ClientFd)
{
	std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Generating Response For Client " << ClientFd << WHIET << std::endl;
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
		std::cerr << YELLOW << currentTime() << RED << " [ERROR] " << "Failed to send response to client " << ClientFd << WHIET << std::endl;
		closeConnection(ClientFd);
		return ;
	}
	std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Response sent to client " << ClientFd << WHIET << std::endl;
}

void Server::checkTimeouts()
{
    time_t now = time(NULL);

    std::map<int, time_t>::iterator it = _lastActivity.begin();
    while (it != _lastActivity.end())
	{
        if (now - it->second > _timeoutSec)
		{
            std::cout << YELLOW << currentTime() << MAGENTA << " [TIMEOUT] Closing " << it->first
                      << " after " << _timeoutSec << "s inactivity" << WHIET << std::endl;
            closeConnection((it++)->first);
        }
		else
        	++it;
    }
}


// ServerConfig Server::getServerConfig()
// {
// 	std::string Host = this->_request.getHeaders()["Host: "];
// 	std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Getting server config for Host: " << Host << WHIET << std::endl;
// 	std::vector<ServerConfig>::iterator it = _config.begin();
// 	for (; it != _config.end(); ++it)
// 	{
// 		std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Checking server config for request" << WHIET << std::endl;
// 		std::cout << RED << "			------------------------------------			" << WHIET << std::endl;
// 		std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Host: " << it->getHost() << WHIET << std::endl;
// 		std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Request Host: " << this->_request.getHeaders().begin()->second << WHIET << std::endl;
// 		std::cout << RED << "			------------------------------------			" << WHIET << std::endl;
// 		if (it->getHost() == this->_request.getHeaders().begin()->first)
// 		{
// 			std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] " << "Matched server config for request" << WHIET << std::endl;
// 			return *it;
// 		}
// 	}
// 	return *(--it);
// }
