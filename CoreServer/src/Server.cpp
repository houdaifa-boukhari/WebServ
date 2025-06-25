/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:55:49 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/06/25 13:34:39 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(const std::vector<ServerConfig>& configs) : _config(configs) , _timeoutSec(30)
{
	this->initializeSockets();
}

Server::~Server()
{
	for (size_t i = 0; i < _serverFds.size(); i++)
    	close(_serverFds[i]);
	for (size_t i = 0; i < _poll_fds.size(); i++)
    	close(_poll_fds[i].fd);
	_poll_fds.clear();
    _serverFds.clear();
	logger::logInfo("Server shutdown, all connections closed.");
}

// check Non-bloking mode

void Server::initializeSockets()
{
	int opt = 1;
	int tmp_fd = 0;
	std::string host;
	std::vector<int> ports;

	for (size_t i = 0; i < _config.size(); i++)
	{
		for (size_t j = 0; j < _config[i].getPorts().size(); j++)
		{
			host = _config[i].getHost();
			ports = _config[i].getPorts();
			if (ports[j] < 1024) // all port younger than 1024 need high privilege
			{
				std::cerr << YELLOW << currentTime() << " [WARNING] " 
          << "Port " << ports[j] << " is privileged. Skipping." <<  WHIET << std::endl;
				continue;
			}
			tmp_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET  IPv4 address family., SOCK_STREAM = TCP protocol (reliable, connection-oriented), 0 = default protocole
			if (tmp_fd == -1)
				throw ("Error creating socket");
			_serverFds.push_back(tmp_fd);
			if (fcntl(tmp_fd, F_SETFL, O_NONBLOCK) == -1)
				throw ("fcntl(F_SETFL) failed");
			if (setsockopt(tmp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) // need to deep understand
    			throw ("setsockopt(SO_REUSEADDR) failed");
			sockaddr_in address;
			address.sin_family = AF_INET;                // Address family: IPv4
			address.sin_addr.s_addr = inet_addr(host.c_str());	// assign host ; why is forebedin
			address.sin_port = htons(ports[j]); 	// assign port
			if (bind(tmp_fd, (struct sockaddr*)&address, sizeof(address)) < 0)  // abin port and host
				perror("bind");
			listen(tmp_fd, SOMAXCONN);
			struct pollfd pfd = {tmp_fd, POLLIN, 0};
			_poll_fds.push_back(pfd);
			std::cout << YELLOW << currentTime() << GREEN << " [INFO] Listening on " << host << ":" << ports[j] << WHIET<< std::endl;
		}
	}
}

void Server::run()
{
	int pollCount = 0;

	while (true)
	{
		checkTimeouts();
		pollCount = poll(_poll_fds.data(), _poll_fds.size(), -1); // first element, size, blockin until event
		if (pollCount < 0)
		{
			std::cout << YELLOW << currentTime() << RED << " [ERROR] " << 
                      "Poll error occurred." << WHIET << std::endl;
            throw ("Poll error");
		}
		size_t i = 0;
		while (i < _poll_fds.size())
		{
			//  Error handling
			if (_poll_fds[i].revents & POLLERR)
			{
        		std::cout << YELLOW << currentTime() << RED << " [ERROR] "
                          << "Closing bad fd=" << _poll_fds[i].fd << WHIET << std::endl;
        		close(_poll_fds[i].fd);
        		_poll_fds.erase(_poll_fds.begin() + i);
        		std::cout << YELLOW << currentTime() << BLUE << " [DEBUG] "
                          << "Erased bad fd. New size=" << _poll_fds.size()  << WHIET << std::endl;
        		continue;
    		}
			// manage connection
			if (_poll_fds[i].revents & POLLIN)
			{
				if (isServerSocket(_poll_fds[i].fd))
				{
					 std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
                              << "Accepting connection on server fd=" << _poll_fds[i].fd << WHIET << std::endl;
    				handleNewConnection(_poll_fds[i].fd);
				}
				else
				{
					std::cout << YELLOW << currentTime() << GREEN << " [INFO] " 
                              << "Reading data from client fd=" << _poll_fds[i].fd << WHIET << std::endl;
    				handleClientData(_poll_fds[i].fd);
				}
			}
			i++;
		}
	}
}

bool Server::isServerSocket(int fd)
{
	for (size_t i = 0; i < _serverFds.size(); i++)
	{
		if (_serverFds[i] == fd)
			return (true);
	}
	return (false);
}
