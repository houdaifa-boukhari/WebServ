/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:33:22 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/06/28 16:02:21 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "logger.hpp"
#include "../../configParsing/serverConfig.hpp"
#include "../../ParseRequest/Request.hpp"
#include "/Users/elmountahi/1337/13Projects/WebServ/response/response.hpp"
#include "/Users/elmountahi/1337/13Projects/WebServ/response/NewResponse.hpp"

#define WHIET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"

class Server
{
private:
	ParssedRequest _request;					// Parsed request
	std::vector<int> _serverFds;				// fds for every socket conections
	std::vector<ServerConfig> _config;			// Parsed server Configuration
	std::vector<struct pollfd> _poll_fds;		// Polling structure for I/O monitoring
	std::map<int, std::string> _client_buffers; // to store request for every client
	std::map<int, time_t> _lastActivity;		// Tracks last activity time per client
	const time_t _timeoutSec;					// Default 30s
public:
	Server(const std::vector<ServerConfig> &configs);
	void initializeSockets();
	void run();
	bool isServerSocket(int fd);
	void handleNewConnection(int SvFd);
	void handleClientData(int ClientFd);
	void closeConnection(int fd);
	void checkTimeouts();
	bool RequestIsComplete(const std::string &request);
	void generateResponse(int ClientFd);
	~Server();
};

#endif