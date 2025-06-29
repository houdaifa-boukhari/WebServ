/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:19:37 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/06/28 22:45:47 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:18:35 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/06/28 21:19:28 by hel-bouk         ###   ########.fr       */
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
		// struct pollfd PollFd;
	public:
		Connection(int fd, const ServerConfig &cfg) : Client_fd(fd), config(cfg), bytesSent(0), lastActivity(time(NULL)) {}

		int getClientFd() const { return Client_fd; }
		const std::string &getClientRequest() const { return ClientRequest; }
		const std::string &getClientResponse() const { return ClientResponse; }
		ssize_t getBytesSent() const { return bytesSent; }
		const ServerConfig &getConfig() const { return config; }
		const ParssedRequest &getParsedRequest() const { return ParseRequest; }
		time_t getLastActivity() const { return lastActivity; }

		void setClientRequest(const std::string &request) { ClientRequest = request; }
		void setClientResponse(const std::string &response) { ClientResponse = response; }
		void setBytesSent(ssize_t bytes) { bytesSent = bytes; }
		void setParsedRequest(const ParssedRequest &parsed) { ParseRequest = parsed; }
		void updateLastActivity() { lastActivity = time(NULL); }
		
		
};





#endif