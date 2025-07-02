/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/07/02 15:30:29 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Connection::reset()
{
	ClientRequest.clear();
	ClientResponse = "";
	lastActivity = time(NULL);
	isComplete = false;
}

void Server::closeConnection(int fd)
{
	if (fd < 0)
		return;
	close(fd);
	_connections[fd].reset();
	_connections.erase(fd);
	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_poll_fds.erase(it);
			break;
		}
	}
	std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
			  << "Closed connection " << fd << WHIET << std::endl;
}

void Server::handleNewConnection(int SvFd)
{
	int ClientFd = 0;
	struct pollfd pfd;
	Connection newConnecion(ClientFd, _server[SvFd]);

	ClientFd = accept(SvFd, NULL, NULL);
	newConnecion.setClientFd(ClientFd);
	if (fcntl(SvFd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(ClientFd);
		throw("fcntl Error");
	}
	pfd.fd = ClientFd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	_poll_fds.push_back(pfd);

	_connections[ClientFd] = newConnecion;
	
}

void Server::handleClientData(int ClientFd)
{
	ssize_t len = 0;
	char buffer[4096];

	if (ClientFd < 0)
		return;
	_connections[ClientFd].updateLastActivity();
	std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
			  << "Reading from client " << ClientFd << WHIET << std::endl;
	len = recv(ClientFd, buffer, 4095, 0); // 0 for non bloking socket
	if (len <= 0)
	{
		if (len == 0)
			std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
					  << "Client " << ClientFd << " disconnected gracefully" << WHIET << std::endl;
		else
		{
			std::cout << YELLOW << currentTime() << RED << " [ERROR] "
					  << "Client " << ClientFd << " has an Error" << WHIET << std::endl;
			closeConnection(ClientFd);
		}
		return;
	}

	buffer[len] = '\0';
	_connections[ClientFd].appendClientRequest(buffer);

	std::cout << GREEN << "\n----------- " << YELLOW << currentTime() << GREEN << " [INFO] "
			  << "Received data from client ------ \n\n"
			  << _connections[ClientFd].getClientRequest() << WHIET << std::endl;

	if (RequestIsComplete(_connections[ClientFd].getClientRequest()))
	{
		
		_connections[ClientFd].setIsComplete(true);

		/////// anour Part ///////

		std::cout << RED << "			------------------------------			" << WHIET << std::endl;
		_connections[ClientFd].getParsedRequest().AssignHeadersLine(_connections[ClientFd].getClientRequest());
		
		std::cout << _connections[ClientFd].getParsedRequest().GetReqeustLIne() << std::endl;
		printMap(_connections[ClientFd].getParsedRequest().getHeaders());
		std::cout << "Body: " << _connections[ClientFd].getParsedRequest().getBody() << std::endl;
		
		_connections[ClientFd].getParsedRequest().assign_params(_connections[ClientFd].getParsedRequest().getBody());

		std::cout << "Params: " << std::endl;
		_connections[ClientFd].getParsedRequest().printParams();
		
		std::cout << RED << "			------------------------------			" << WHIET << std::endl;
		_connections[ClientFd].getResponse().setParssedRequest(_connections[ClientFd].getParsedRequest());
		
		// char **env = _request.get_env();
		// std::string output = execute_cgi("/Users/aet-tale/Desktop/webserv/cgi-cookies.py", env);
		// std::cout << "Output: " << output << std::endl;
		// free_envp(env);

		// NewResponse response(ClientFd, _config[0], _request);
		// response.generateResponse();

		
		// std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
		// 		  << "Request Is complete" << WHIET << std::endl;
		// std::cout << RED << "			------------------------------------			" << std::endl;
	}
	else
		std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
				  << "Request not complete yet" << WHIET << std::endl;
}



SendStatus Connection::getSendStatus() const
{
	return _response.getSendStatus();
}

void Connection::generateResponse()
{
	std::cout << "starting  generateResponse" << std::endl;
	this->_response.generateResponse();
}

void Connection::generateChunkedResponse()
{
	this->_response.sendNextChunk();
}