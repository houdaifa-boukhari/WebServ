/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/05/09 11:59:42 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

void Server::closeConnection(int fd)
{
	if (fd < 0)
		return ;
	close(fd);
	 _client_buffers.erase(fd);
	 _lastActivity.erase(fd);
	for (size_t i = 0; i < _poll_fds.size(); i++)
	{
		if (_poll_fds[i].fd == fd)
		{
			_poll_fds.erase(_poll_fds.begin() + i);
			return ;
		}
	}
	std::cout << currentTime() << " [INFO] Closed connection " << fd << std::endl;
}
#include <iostream>     // std::cout

void Server::handleNewConnection(int SvFd)
{
	int ClientFd = 0;
	struct pollfd pfd;

	ClientFd = accept(SvFd, NULL, NULL);
	if (fcntl(SvFd, F_SETFL, O_NONBLOCK) < 0)
		throw ("fcntl Error");
	pfd.fd = ClientFd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	_poll_fds.push_back(pfd);
	_client_buffers[ClientFd] = "";
}
#include <fstream>
void Server::handleClientData(int ClientFd)
{
	ssize_t len = 0;
	char buffer[4096];

	if (ClientFd < 0)
		return ;
	_lastActivity[ClientFd] = time(NULL);
	std::cout << currentTime() << " [INFO] Reading from client " << ClientFd << std::endl;
	len = recv(ClientFd, buffer, 4095, 0); // 0 for non bloking socket
	if (len <= 0)
	{
		if (len == 0)
			std::cout << currentTime() << " [INFO] " << "Client " << ClientFd << " disconnected gracefully" << std::endl;
		else
			std::cerr << currentTime() << " [ERROR] " << "Client " << ClientFd << " has an Error" << std::endl;
		closeConnection(ClientFd);
		return ;
	}
	
	buffer[len] = '\0';
	_client_buffers[ClientFd].append(buffer, len);

	std::cout << "\n-----------" << currentTime() << " [INFO] " << "Received data from client ------ \n\n" << _client_buffers[ClientFd] << std::endl;
	
	// std::cout << "\n-----------" << currentTime() << " [INFO] " << "Received data from client ------ \n\n";

	if (RequestIsComplete(_client_buffers[ClientFd]))
	{
		std::string name = "request.http";
    	std::ofstream ofs;
		
		ofs.open (name.c_str());
		ofs << _client_buffers[ClientFd];

		/// parse the request here

		
		generateResponse(ClientFd);
		_client_buffers[ClientFd].clear();
		_client_buffers[ClientFd] = "";
		std::cout << currentTime() << " [INFO] " << "Request Is complete" << std::endl;
		std::cout << currentTime() << "------------------------------------" << std::endl;
	}
	else
		std::cout << currentTime() << " [INFO] " << "Request not complete yet" << std::endl;
}
