/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:33:22 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/05/06 21:25:32 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../../configParsing/serverConfig.hpp"

class Server
{
	private :
		std::vector<int> _serverFds; // fds for every socket conections
		std::vector<ServerConfig> _config; // Parsed server Configuration
		std::vector<struct pollfd>   _poll_fds;  // Polling structure for I/O monitoring
	public :
		Server(const std::vector<ServerConfig>& configs);
		void initilizeSockets();
		~Server();
};

#endif