/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:19:37 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/07/02 12:54:59 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include "../../ParseRequest/Request.hpp"
#include "../../configParsing/serverConfig.hpp"
#include "../../response/NewResponse.hpp"

class Connection
{
private:
	int Client_fd;
	std::string ClientRequest;
	std::string ClientResponse;
	ssize_t bytesSent;
	size_t totalFileSize;
	ServerConfig config;
	ParssedRequest _request;
	NewResponse _response;
	time_t lastActivity;
	bool isComplete;

public:
	Connection();
	Connection(int fd, ServerConfig &cfg);
	int getClientFd() const { return Client_fd; }
	const std::string &getClientRequest() const { return ClientRequest; }
	const std::string &getClientResponse() const { return ClientResponse; }
	ssize_t getBytesSent() const { return bytesSent; }
	time_t getLastActivity() const { return lastActivity; }
	bool getIsComplete() const { return isComplete; }
	ParssedRequest getParceRequest() { return _request; }

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
	void setParsedRequest(const ParssedRequest &parsed) { _request = parsed; }
	void updateLastActivity() { lastActivity = time(NULL); }

	// Response related methods
	SendStatus getSendStatus() const;
	void generateResponse();
	void generateChunkedResponse();
};

#endif