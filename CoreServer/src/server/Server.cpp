/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:55:49 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/05/07 22:37:17 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

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
				std::cerr << "[WARNING] Port " << ports[j] << " is privileged. Skipping.\n";
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
			std::cout << "[INFO] Listening on " << host << ":" << ports[j] << std::endl;
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
			throw ("Poll error");
		size_t i = 0;
		while (i < _poll_fds.size())
		{
			//  Error handling
			if (_poll_fds[i].revents & POLLERR)
			{
        		std::cout << "Closing bad fd=" << _poll_fds[i].fd << std::endl;
        		close(_poll_fds[i].fd);
        		_poll_fds.erase(_poll_fds.begin() + i);
        		std::cout << "Erased - new size=" << _poll_fds.size() << std::endl;
        		continue;
    		}
			// manage connection
			if (_poll_fds[i].revents & POLLIN)
			{
				if (isServerSocket(_poll_fds[i].fd))
				{
					std::cout << "Accepting on server fd=" << _poll_fds[i].fd << std::endl;
    				handleNewConnection(_poll_fds[i].fd);
				}
				else
				{
					 std::cout << "Reading from client fd=" << _poll_fds[i].fd << std::endl;
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

void Server::closeConnection(int fd)
{
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

void Server::handleClientData(int ClientFd)
{
	size_t len = 0;
	char buffer[4096];

	_lastActivity[ClientFd] = time(NULL);
	std::cout << currentTime() << " [INFO] Reading from client " << ClientFd << std::endl;
	len = recv(ClientFd, buffer, sizeof(buffer), 0); // 0 for non bloking socket
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

	if (RequestIsComplete(_client_buffers[ClientFd]))
	{
		std::cout << currentTime() << " [INFO] " << "Request complete" << std::endl;
		generateResponse(ClientFd);
	}
	else
		std::cout << currentTime() << " [INFO] " << "Request not complete yet" << std::endl;
}

void Server::checkTimeouts()
{
    time_t now = time(NULL);

    std::map<int, time_t>::iterator it = _lastActivity.begin();
    while (it != _lastActivity.end())
	{
        if (now - it->second > _timeoutSec)
		{
            std::cout << currentTime() << " [TIMEOUT] Closing " << it->first
                      << " after " << _timeoutSec << "s inactivity\n";
            closeConnection((it++)->first);
        }
		else
        	++it;
    }
}
bool Server::RequestIsComplete(const std::string& request)
{
	size_t pos, start, end, len, body_start, body_size;
	std::string header, contentLengthStr;

	// Check if headers are complete
	pos = request.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		std::cerr << currentTime() << " [ERROR] " << "Headers not complete yet" << std::endl;
		return (false);
	}
	// Get first line (request line)
	size_t line_end = request.find("\r\n");
	if (line_end == std::string::npos)
		return (false);
	header = request.substr(0, line_end);

	if (header.find("GET") == 0 || header.find("DELETE") == 0)
		return (true);
	if (header.find("POST") == 0)
	{
		pos = request.find("Content-Length:");
		if (pos == std::string::npos)
			return (false);
		start = pos + 15;
		while (start < request.size() && (request[start] == ' ' || request[start] == '\t'))
			start++;
		end = request.find("\r\n", start);
		if (end == std::string::npos)
			return (false);
		contentLengthStr = request.substr(start, end - start);
		len = ::atoi(contentLengthStr.c_str());
		// Body starts right after \r\n\r\n
		body_start = request.find("\r\n\r\n") + 4;
		body_size = request.size() - body_start;
		if (body_size < len)
			return (false);
		return (true);
	}
	return (false);
}

void Server::generateResponse(int ClientFd)
{
	std::string    exampleHtml = "<html><body><h1> <center> Welcome to 1337 | testing Webserv </center></h1></body></html>";
    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: " + std::to_string(exampleHtml.size()) + "\r\n"
                           "Access-Control-Allow-Origin: *\r\n" 
                           "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n" 
                           "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                           "\r\n" + exampleHtml;
	send(ClientFd, response.c_str(), response.size(), 0);
}