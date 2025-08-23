/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:19:37 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/23 13:51:12 by hel-bouk         ###   ########.fr       */
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
		bool KeepAlive;
		bool needClose;
		size_t sizeToRead;
	public:
 		Connection()
            : Client_fd(-1),
              ClientRequest(),
              ClientResponse(""),
              config(),
              ParseRequest(),
              _response(),
              lastActivity(time(NULL)),
              isComplete(false),
              KeepAlive(false),
              needClose(false),
			  sizeToRead(4096) {}
		  Connection(int fd, const ServerConfig &cfg)
            : Client_fd(fd),
              ClientRequest(),
              ClientResponse(""),
              config(cfg),
              ParseRequest(),
              _response(),
              lastActivity(time(NULL)),
              isComplete(false),
              KeepAlive(false),
              needClose(false),
			  sizeToRead(4096) {}
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
		void setClientResponse(const std::string &response) { ClientResponse = response; }
		void setParsedRequest(const ParssedRequest &parsed) { ParseRequest = parsed; }
		void updateLastActivity() { lastActivity = time(NULL); }
		bool getNeedClose() {return needClose;};
		void setNeedClose(bool status) {needClose = status;}
		bool getKeepAlive() {return KeepAlive;}
		void setKeepAlive(bool status) {KeepAlive = status;}
		bool RequestIsComplete(const std::vector<char> &request);
		void buildResponse(int clientFd, ParssedRequest &request, ServerConfig &config);
		SendStatus getSendStatus() const;
		void generateResponse();
		void generateChunkedResponse();
};

#endif