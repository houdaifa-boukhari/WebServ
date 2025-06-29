/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:19:37 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/06/29 13:06:55 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include "../../ParseRequest/Request.hpp"
#include "../../configParsing/serverConfig.hpp"

class Connection
{
	private:
		int Client_fd;
		std::string ClientRequest;
		std::string ClientResponse;
		ssize_t bytesSent;
		ServerConfig config;
		ParssedRequest ParseRequest;
		time_t lastActivity;
		bool isComplete;
	public:
		Connection() : Client_fd(-1), bytesSent(0), lastActivity(time(NULL)), ClientRequest(""), ClientResponse(""), isComplete(false) {}
		Connection(int fd, const ServerConfig &cfg) : Client_fd(fd), config(cfg), bytesSent(0), lastActivity(time(NULL)), ClientRequest(""), ClientResponse(""), isComplete(false) {}

		int getClientFd() const { return Client_fd; }
		const std::string &getClientRequest() const { return ClientRequest; }
		const std::string &getClientResponse() const { return ClientResponse; }
		ssize_t getBytesSent() const { return bytesSent; }
		time_t getLastActivity() const { return lastActivity; }
		bool getIsComplete() const { return isComplete; }
		
		void reset();
		void setIsComplete(bool complete) { isComplete = complete; }
		void setClientFd(int fd) { Client_fd = fd; }
		void clearClientRequest() { ClientRequest.clear(); }
		void clearClientResponse() { ClientResponse.clear(); }
		void appendClientRequest(const std::string &request) { ClientRequest.append(request); }
		void appendClientResponse(const std::string &response) { ClientResponse.append(response); }
		void setClientRequest(const std::string &request) { ClientRequest = request; }
		void setClientResponse(const std::string &response) { ClientResponse = response; }
		void setBytesSent(ssize_t bytes) { bytesSent = bytes; }
		void setParsedRequest(const ParssedRequest &parsed) { ParseRequest = parsed; }
		void updateLastActivity() { lastActivity = time(NULL); }
		
};





#endif