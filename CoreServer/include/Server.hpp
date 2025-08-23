/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 10:11:38 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/23 10:45:41 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <fstream>
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
#include "../../response/NewResponse.hpp"
#include "Connection.hpp"

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
		std::map<int, Connection> _connections;
		std::map<int, ServerConfig> _server;
		std::vector<ServerConfig> _config;
		std::vector<struct pollfd> _poll_fds;
		const time_t _timeoutSec;
	public:
		Server(const std::vector<ServerConfig> &configs);
		void initializeSockets();
		void run(); 
		bool isServerSocket(int fd);
		void handleNewConnection(int SvFd);
		void handleClientData(int ClientFd);
		void closeConnection(int fd);
		void checkTimeouts();
		ssize_t  InetAdress(std::string Ip);

		
		~Server();
};

#endif