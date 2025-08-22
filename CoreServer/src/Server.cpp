/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:55:49 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/22 20:53:42 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(const std::vector<ServerConfig> &configs) : _config(configs), _timeoutSec(120)
{
	this->initializeSockets();
}

Server::~Server()
{
	std::map<int, ServerConfig>::iterator it;

	for (it = _server.begin(); it != _server.end(); ++it)
		close(it->first);
	for (size_t i = 0; i < _poll_fds.size(); i++)
		close(_poll_fds[i].fd);
	_poll_fds.clear();
	_server.clear();
	logger::logInfo("Server shutdown, all connections closed.");
}

void Server::initializeSockets()
{
	int opt = 1;
	int tmp_fd = 0;
	std::string host;
	std::vector<int> ports;

	for (size_t i = 0; i < _config.size(); i++)
	{
		host = _config[i].getHost();
		ports = _config[i].getPorts();
		for (size_t j = 0; j < _config[i].getPorts().size(); j++)
		{
			if (ports[j] < 1024) // all port younger than 1024 need high privilege
			{
				std::cerr << YELLOW << currentTime() << " [WARNING] "
						  << "Port " << ports[j] << " is privileged. Skipping." << WHIET << std::endl;
				continue;
			}
			tmp_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET  IPv4 address family., SOCK_STREAM = TCP protocol (reliable, connection-oriented), 0 = default protocole
			if (tmp_fd == -1)
				throw("Error creating socket");
			_server[tmp_fd] = _config[i];
			if (fcntl(tmp_fd, F_SETFL, O_NONBLOCK) == -1)
				throw("fcntl(F_SETFL) failed");
			if (setsockopt(tmp_fd, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) < 0) // need to deep understand
				throw("setsockopt(SO_REUSEADDR) failed");
			setsockopt(tmp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
			sockaddr_in address;
			address.sin_family = AF_INET;										// Address family: IPv4
			address.sin_addr.s_addr = InetAdress(host);
 			address.sin_port = htons(ports[j]);									// assign port
			if (bind(tmp_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
			{
				std::cout << YELLOW << currentTime() << RED << " [ERROR] Is Already Listening " << host << ":" << ports[j] << WHIET << std::endl;
				continue ;
			}
			listen(tmp_fd, SOMAXCONN);
			struct pollfd pfd = {tmp_fd, POLLIN, 0};
			_poll_fds.push_back(pfd);
			std::cout << YELLOW << currentTime() << GREEN << " [INFO] Listening on " << host << ":" << ports[j] << WHIET << std::endl;
		}
	}
}

void Server::run()
{
	int pollCount = 0;
	bool tmp = false;

	while (true)
	{
		checkTimeouts();
		pollCount = poll(_poll_fds.data(), _poll_fds.size(), -1); // first element, size, blockin until event
		if (pollCount < 0)
		{
			std::cout << YELLOW << currentTime() << RED << " [ERROR] "
					  << "Poll error occurred." << WHIET << std::endl;
			throw("Poll error");
		}
		size_t i = 0;
		while (i < _poll_fds.size())
		{
			if (_poll_fds[i].revents & POLLERR)
			{
				std::cout << YELLOW << currentTime() << RED << " [ERROR] "
						  << "Closing bad fd=" << _poll_fds[i].fd << WHIET << std::endl;
				close(_poll_fds[i].fd);
				_poll_fds.erase(_poll_fds.begin() + i);
				std::cout << YELLOW << currentTime() << BLUE << " [DEBUG] "
						  << "Erased bad fd. New size=" << _poll_fds.size() << WHIET << std::endl;
				continue;
			}
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
					// std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
					// 		  << "Reading data from client fd=" << _poll_fds[i].fd << WHIET << std::endl;
					handleClientData(_poll_fds[i].fd);
				}
			}
			else if (_poll_fds[i].revents & POLLOUT && (_connections[_poll_fds[i].fd].getIsComplete() || _connections[_poll_fds[i].fd].getSendStatus() == SEND_IN_PROGRESS)) // remove else (check if close connection)
			{
				if (!isServerSocket(_poll_fds[i].fd))
				{
					std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
							  << "Sending data to client fd=" << _poll_fds[i].fd << WHIET << std::endl;
					// _connections[_poll_fds[i].fd].setIsComplete(false);
					if (_connections[_poll_fds[i].fd].getSendStatus() == SEND_NOT_STARTED)
						_connections[_poll_fds[i].fd].generateResponse();
					else if (_connections[_poll_fds[i].fd].getSendStatus() == SEND_IN_PROGRESS)
						_connections[_poll_fds[i].fd].generateChunkedResponse();
					else if (_connections[_poll_fds[i].fd].getSendStatus() == SEND_ERROR)
						std::cerr << YELLOW << currentTime() << RED << " [ERROR] "
								  << "Error sending data to client fd=" << _poll_fds[i].fd << WHIET << std::endl;
					if (_connections[_poll_fds[i].fd].getKeepAlive())
					{
						std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
								  << "Keeping connection alive for client fd=" << _poll_fds[i].fd << " , Untile Timeout" << WHIET << std::endl;
						tmp = _connections[_poll_fds[i].fd].getKeepAlive();
						_connections[_poll_fds[i].fd].reset();
						_connections[_poll_fds[i].fd].setKeepAlive(tmp);
						
					}
					else if (_connections[_poll_fds[i].fd].getSendStatus() == SEND_COMPLETED)
					{
						std::cout << YELLOW << currentTime() << CYAN << " [DEBUG] "
								  << "All Response send it to " << _poll_fds[i].fd << WHIET << std::endl;
						std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
								  << "Closing connection for client fd=" << _poll_fds[i].fd << WHIET << std::endl;								  
						tmp = _connections[_poll_fds[i].fd].getKeepAlive();
						_connections[_poll_fds[i].fd].reset();
						if (!tmp)
							closeConnection(_poll_fds[i].fd);
						else
							_connections[_poll_fds[i].fd].setKeepAlive(tmp);
							
					}
				}
			}
			i++;
		}
	}
}

bool Server::isServerSocket(int fd)
{
	if (_server.find(fd) != _server.end())
		return (true);
	return (false);
}
