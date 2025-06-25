/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/06/25 15:40:03 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Server::closeConnection(int fd)
{
	if (fd < 0)
		return ;
	close(fd);
	 _client_buffers.erase(fd);
	 _lastActivity.erase(fd);
 	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
    {
        if (it->fd == fd)
        {
            _poll_fds.erase(it);
            break;
    	}
	}
	std::cout << YELLOW << currentTime() << GREEN << " [INFO] " << "Closed connection " << fd << WHIET << std::endl;
}

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
	std::cout << YELLOW << currentTime() << GREEN << " [INFO] " << "Reading from client " << ClientFd << WHIET << std::endl;
	len = recv(ClientFd, buffer, 4095, 0); // 0 for non bloking socket
	if (len <= 0)
	{
		if (len == 0)
			std::cout << YELLOW << currentTime() << GREEN << " [INFO] " << "Client " << ClientFd << " disconnected gracefully" << WHIET << std::endl;
		else
			std::cout << YELLOW << currentTime() << RED << " [ERROR] " << "Client " << ClientFd << " has an Error" <<  WHIET << std::endl;
		closeConnection(ClientFd);
		return ;
	}
	
	buffer[len] = '\0';
	_client_buffers[ClientFd].append(buffer, len);

	std::cout << GREEN << "\n----------- " << YELLOW << currentTime() << GREEN << " [INFO] " << "Received data from client ------ \n\n" << _client_buffers[ClientFd] << WHIET<< std::endl;
	
	if (RequestIsComplete(_client_buffers[ClientFd]))
	{

		/////// anour Part ///////

		std::cout << "\033[1;31m********\033[0m" << std::endl;
		_request.AssignHeadersLine(_client_buffers[ClientFd]);
		std::cout << _request.GetReqeustLIne() << std::endl;
		printMap(_request.getHeaders());
		std::cout << "Body: " << _request.getBody() << std::endl;
		_request.assign_params(_request.getBody());
		std::cout << "Params: " << std::endl;
		_request.printParams();
		std::cout << "\033[1;31m********\033[0m" << std::endl;
		// char **env = _request.get_env();
		// std::string output = execute_cgi("/Users/aet-tale/Desktop/webserv/cgi-cookies.py", env);
		// std::cout << "Output: " << output << std::endl;
		// free_envp(env);

		/////////////////////////////		

		///// youssef Part /////


		// generatResponse(serverConfig, _request, ClientFd);

		//////////////////
		generateResponse(ClientFd);
		_client_buffers[ClientFd].clear();
		_client_buffers[ClientFd] = "";
		// _config.;
		std::cout << YELLOW << currentTime() << GREEN << " [INFO] " << "Request Is complete" << WHIET << std::endl;
		std::cout << RED << "			------------------------------------			" << std::endl;
	}
	else
		std::cout << YELLOW << currentTime() << GREEN << " [INFO] " << "Request not complete yet" << WHIET << std::endl;
}
