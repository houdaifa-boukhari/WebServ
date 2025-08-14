/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:19:37 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/09 20:13:15 by hel-bouk         ###   ########.fr       */
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
		std::vector<char> ClientRequest;
		std::string ClientResponse;
		ServerConfig config;
		ParssedRequest ParseRequest;
		NewResponse _response;
		time_t lastActivity;
		bool isComplete;
	public:
		Connection() : Client_fd(-1), lastActivity(time(NULL)), ClientResponse(""), isComplete(false) {}
		Connection(int fd, const ServerConfig &cfg) : Client_fd(fd), config(cfg), lastActivity(time(NULL)), ClientResponse(""), isComplete(false) {}
		int getClientFd() const { return Client_fd; }
		std::string getClientRequest()  {
			return std::string(ClientRequest.data(), ClientRequest.size()); }
		const std::vector<char> &getClientRequestVector() const { return ClientRequest; }
		const std::string &getClientResponse() const { return ClientResponse; }
		time_t getLastActivity() const { return lastActivity; }
		bool getIsComplete() const { return isComplete; }
		ParssedRequest &getParceRequest() {return ParseRequest;}
		void reset();
		ServerConfig &getServerConfig() { return config; }
		NewResponse &getResponse() { return _response; }
		ParssedRequest &getParsedRequest() { return ParseRequest; }
		void setIsComplete(bool complete) { isComplete = complete; }
		void setClientFd(int fd) { Client_fd = fd; }
		void clearClientRequest() { ClientRequest.clear(); }
		void clearClientResponse() { ClientResponse.clear(); }
		void appendClientRequest(const std::vector<char> request) {ClientRequest.insert(ClientRequest.end(), request.begin(), request.end()); }
		void appendClientResponse(const std::string &response) { ClientResponse.append(response); }
		// void setClientRequest(const std::string &request) { ClientRequest = request; }
		void setClientResponse(const std::string &response) { ClientResponse = response; }
		void setParsedRequest(const ParssedRequest &parsed) { ParseRequest = parsed; }
		void updateLastActivity() { lastActivity = time(NULL); }

	bool RequestIsComplete(const std::vector<char> &request);
		// Response related methods
	void buildResponse(int clientFd, ParssedRequest &request, ServerConfig &config);
	SendStatus getSendStatus() const;
	void generateResponse(); //Add commentMore actions
	void generateChunkedResponse();
		
};

#endif