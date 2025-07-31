/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/07/31 16:47:00 by yel-moun         ###   ########.fr       */
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

void print_map(const std::map<std::string, std::string> &m)
{
	for (std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		std::cout << it->first << " = " << it->second << std::endl;
	}
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
		{
			std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
					  << "Client " << ClientFd << " disconnected gracefully" << WHIET << std::endl;
		}
		else
		{
			std::cout << YELLOW << currentTime() << RED << " [ERROR] "
					  << "Client " << ClientFd << " has an Error" << WHIET << std::endl;
		}
		closeConnection(ClientFd);
		return;
	}

	buffer[len] = '\0';
	_connections[ClientFd].appendClientRequest(buffer);

	// std::cout << GREEN << "\n----------- " << YELLOW << currentTime() << GREEN << " [INFO] "
	// 		  << "Received data from client ------ \n\n"
	// 		  << _connections[ClientFd].getClientRequest() << WHIET << std::endl;

	if (_connections[ClientFd].RequestIsComplete(_connections[ClientFd].getClientRequest()))
	{
		_connections[ClientFd].setIsComplete(true);

		/////// anour Part ///////
		ParssedRequest &request = _connections[ClientFd].getParceRequest();
		std::cout << RED << "			------------------------------			" << WHIET << std::endl;
		request.AssignHeadersLine(_connections[ClientFd].getClientRequest());
		request.printParams();
		if (request.is_cgi() && (request.getMethod() == "GET" || request.getMethod() == "POST"))
		{
			request.assign_full_cgi_path();
			if (request.path_exists())
			{
				char **env = request.get_env();
				request.assign_cgi_output(execute_cgi(request.get_cgi_path(), env));
				std::cout << GREEN << "output_cgi : " << std::endl
						  << request.get_cgi_output() << WHIET << std::endl;
				free_envp(env);
			}
			else
				std::cout << "not a correct cgi path" << std::endl;
		}
		else
			std::cout << "normal request" << std::endl;
		std::cout << RED << "			------------------------------			" << WHIET << std::endl;
		// std::cout << request.getBody() << std::endl;
		// print_map(request.getHeaders());
		// std::cout << RED << "			------------------------------			" << WHIET << std::endl;

		if (request.get_cookie_avai())
		{
			// maybe send only the ones that aren't ava
			// std::cout << "the cookie bg-color has been send in the req" << std::endl;
			// request.assign_cookies_response(execute_cgi("/Users/aet-tale/Desktop/WebServ/cookie_set.py", NULL));
			// request.assign_cookies_response(execute_cgi("/Users/aet-tale/Desktop/WebServ/session_set.py", NULL));
			// std::cout << request.get_cookies_response() << std::endl;
		}
		std::cout << "print cookies" << std::endl;
		print_map(request.get_cookies());
		// else no cookies should be sent
		ParssedRequest req = _connections[ClientFd].getParsedRequest();
		ServerConfig confg = _connections[ClientFd].getServerConfig();
		// std::cout << confg.getHost() << ":" << std::endl;
		_connections[ClientFd].buildResponse(ClientFd, req, confg);

		// std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
		//		<< "Request Is complete" << WHIET << std::endl;
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
	this->_response.generateResponse();
}

void Connection::generateChunkedResponse()
{
	this->_response.sendNextChunk();
}

void Connection::buildResponse(int clientFd, ParssedRequest &request, ServerConfig &config)
{
	_response = NewResponse(clientFd, config, request);
}