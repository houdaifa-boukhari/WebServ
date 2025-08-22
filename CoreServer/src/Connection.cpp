/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/19 12:31:42 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Connection::reset()
{
	ClientRequest.clear();
	ClientResponse = "";
	lastActivity = time(NULL);
	isComplete = false;
	KeepAlive = false;
	needClose = false;
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
	std::vector<char> buffer(1024);

	if (ClientFd < 0)
		return;
	_connections[ClientFd].updateLastActivity();
	std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
			  << "Reading from client " << ClientFd << WHIET << std::endl;
	len = recv(ClientFd, buffer.data(), buffer.size(), 0); // 0 for non bloking socket
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
	buffer.resize(len);
	_connections[ClientFd].appendClientRequest(buffer);

	// std::cout << GREEN << "\n----------- " << YELLOW << currentTime() << GREEN << " [INFO] "
	// 		  << "Received data from client ------ \n\n"
	// 		  << _connections[ClientFd].getClientRequest() << WHIET << std::endl;

	if (_connections[ClientFd].RequestIsComplete(_connections[ClientFd].getClientRequestVector()))
	{
		_connections[ClientFd].setIsComplete(true);

		/////// anour Part ///////
		ParssedRequest &request = _connections[ClientFd].getParceRequest();
		std::cout << RED << "			------------------------------	l		" << WHIET << std::endl;
		request.AssignHeadersLine(_connections[ClientFd].getClientRequest());
		if (request.get_correct_size() == false)
		{
			NewResponse::sendSimpleErrorResponse(ClientFd, 400, _connections[ClientFd].getServerConfig());
			_connections[ClientFd].setIsComplete(false);
			closeConnection(ClientFd);
			return;
		}
		if (request.is_cgi() && (request.getMethod() == "GET" || request.getMethod() == "POST"))
		{
			request.assign_full_cgi_path();
			if (request.path_exists())
			{
				char **env = request.get_env();
				request.assign_cgi_output(execute_cgi(request, request.get_cgi_path(), env));
				std::cout << GREEN << "output_cgi : " << std::endl << request.get_cgi_output() << WHIET << std::endl;
				std::cout << "output" << execute_cgi(request, request.get_cgi_path(), env) << std::endl;
				free_envp(env);
			}
			else
			{
				request.set_status_code(404);
				std::cout << "not a correct cgi path" << std::endl;
			}
		}
		// else
		// std::cout << "normal request" << std::endl;

		ParssedRequest req = _connections[ClientFd].getParsedRequest();
		ServerConfig confg = _connections[ClientFd].getServerConfig();
		if (_connections[ClientFd].getClientRequest().find("Connection: keep-alive"))
			_connections[ClientFd].setKeepAlive(true);
		_connections[ClientFd].buildResponse(ClientFd, req, confg);
	}
	else if (_connections[ClientFd].getNeedClose())
		closeConnection(ClientFd);
	// 	std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
	// 			  << "Request not complete yet" << WHIET << std::endl;
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
