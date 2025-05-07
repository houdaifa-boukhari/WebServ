/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:55:49 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/05/06 21:31:51 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

Server::Server(const std::vector<ServerConfig>& configs) : _config(configs)
{
	this->initilizeSockets();
}

Server::~Server()
{
	
}

// check Non-bloking mode

void Server::initilizeSockets()
{
	int tmp_fd = 0, flag = 0;

	for (size_t i = 0; i < _config.size(); i++)
	{
		for (int j = 0; j < _config[i].getPorts()[j]; j++)
		{
			tmp_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET  IPv4 address family., SOCK_STREAM = TCP protocol (reliable, connection-oriented), 0 = default protocole
			if (tmp_fd == -1)
				throw ("Error creating socket");
			_serverFds.push_back(tmp_fd);
			flag = fcntl(tmp_fd, F_GETFL);
			if (flag == -1)
				throw ("fcntl(F_GETFL) failed");
			if (fcntl(tmp_fd, F_SETFD, flag | O_NONBLOCK) == -1)
				throw ("fcntl(F_SETFD) failed");
			sockaddr_in address;
			address.sin_family = AF_INET;                // Address family: IPv4
			address.sin_addr.s_addr = inet_addr(_config[i].getHost().c_str());	// assign host
			address.sin_port = htons(_config[i].getPorts()[j]); 	// assign port
			if (bind(tmp_fd, (struct sockaddr*)&address, sizeof(address)) < 0)  // abin port and host
				throw ("bind error");
			listen(tmp_fd, SOMAXCONN);
			struct pollfd pfd = {tmp_fd, POLLIN, 0};
			_poll_fds.push_back(pfd);
		}
	}
}
