/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:20:09 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/08/09 20:30:28 by hel-bouk         ###   ########.fr       */
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

LocationConfig* findMatchingLocation(std::vector<LocationConfig> &locations,const std::string &pat)
{
    // std::vector<LocationConfig> locations = _config.getLocations();
    LocationConfig *bestMatch = NULL;
    size_t bestLen = 0;
    bool isWildcardMatch = false;
	std::string path;
	if (pat == "/")
		path = pat;
	else 
		path = pat.substr(1);
		
    for (size_t i = 0; i < locations.size(); i++)
    {
        const std::string &locName = locations[i].getName();

        if (locName.size() > 1 && locName[0] == '*' && locName[1] == '.')
        {
            std::string extension = locName.substr(1); // Remove the '*'
            if (path.size() >= extension.size() &&
                path.substr(path.size() - extension.size()) == extension)
            {
                if (!bestMatch || !isWildcardMatch || extension.size() > bestLen)
                {
                    bestLen = extension.size();
                    bestMatch = &locations[i];
                    isWildcardMatch = true;
                }
            }
        }
        else if (path.compare(0, locName.size(), locName) == 0 &&
                 (path.size() == locName.size() || path[locName.size()] == '/' || locName[locName.size() - 1] == '/'))
        {
            if (!isWildcardMatch && locName.size() > bestLen)
            {
                bestLen = locName.size();
                bestMatch = &locations[i];
            }
        }
    }

    if (bestMatch)
    {
		std::cout << "correct" << std::endl;
        // this->_matchedLocation = *bestMatch;
        return bestMatch;
    }
    return NULL;
}

// LocationConfig* findMatchingLocation(std::vector<LocationConfig> &locations,const std::string &pat)
// {
//     LocationConfig *bestMatch = NULL;
//     size_t bestLen = 0;
// 	std::string path = pat.substr(1); // Remove query parameters if any
//     for (size_t i = 0; i < locations.size(); i++)
//     {
//         const std::string &locName = locations[i].getName();
//         if (path.compare(0, locName.size(), locName) == 0 &&
//             (path.size() == locName.size() || path[locName.size()] == '/' || locName[locName.size() - 1] == '/'))
//         {
//             if (locName.size() > bestLen)
//             {
//                 bestLen = locName.size();
//                 bestMatch = &locations[i];
//             }
//         }
//     }
//     if (bestMatch)
//     {
// 		std::cout << GREEN << currentTime() << GREEN << " [INFO] "
// 				  << "Found matching location: " << bestMatch->getName() << WHIET << std::endl;
//        return  bestMatch;
//     }
// 	std::cout << GREEN << currentTime() << RED << " [ERROR] "
// 			  << "No matching location found for path: " << path << WHIET << std::endl;
//     return NULL;
// }

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
		// std::cout << findMatchingLocation << std::endl;
		std::cout << RED << "			------------------------------	l		" << WHIET << std::endl;
		request.AssignHeadersLine(_connections[ClientFd].getClientRequest());
		// if (CgiConfig != NULL)
		// 	std::cout << YELLOW << "HHHHHHH " << CgiConfig->getCgiPath() << " ddd" << WHIET << std::endl;
		// else
		// {
		// 	request.set_status_code(404);
		// 	std::cout << YELLOW << "HHHHHHH NULL PATH" <<  WHIET << std::endl;
		// }
		// std::cout << YELLOW << request.get_path_without_params() << WHIET << std::endl;

		if (request.is_cgi() && (request.getMethod() == "GET" || request.getMethod() == "POST"))
		{
			ServerConfig &config = _connections[ClientFd].getServerConfig();
			std::vector<LocationConfig> locations = config.getLocations();
			LocationConfig *CgiConfig = findMatchingLocation(locations, request.get_path_without_params());
			request.assign_cgi_path(CgiConfig->getCgiPath());
			request.assign_full_cgi_path();
			if (request.path_exists() && CgiConfig != NULL)
			{
				char **env = request.get_env();
				request.assign_cgi_output(execute_cgi(request, request.get_cgi_path(), env));
				// std::cout << GREEN << "output_cgi : " << std::endl << request.get_cgi_output() << WHIET << std::endl;
				// std::cout << "output" << execute_cgi(request, request.get_cgi_path(), env) << std::endl;
				free_envp(env);
			}
			else
			{
				request.set_status_code(404);
				std::cout << "not a correct cgi path" << std::endl;
			}
		}
		else
			std::cout << "normal request" << std::endl;
		std::cout << RED << "			------------------------------	l		" << WHIET << std::endl;
		std::cout << MAGENTA << "cookies : " << WHIET << std::endl;
		print_map(request.get_cookies());
		std::cout << MAGENTA << "params : " << WHIET << std::endl;
		print_map(request.getParams());
		std::cout << MAGENTA << "raw path : " << WHIET  << std::endl;
		std::cout << request.get_path_without_params() << std::endl;
		std::cout << MAGENTA << "raw request : "<< WHIET  << std::endl;
		std::cout << request.getPath()	<< std::endl;
		std::cout << MAGENTA << "headers : "<< WHIET  << std::endl;
		print_map(request.getHeaders());
		std::cout << MAGENTA << "content type value : "<< WHIET << std::endl;
		print_map(request.get_content_type_values());
		std::cout << MAGENTA << "get boundary : "<< WHIET << std::endl;
		std::cout << request.get_boundary() << std::endl;
		std::cout << MAGENTA << "body : "<< WHIET  << std::endl;
		std::cout << request.getBody() << std::endl;
		// std::cout << request.parse_body() << std::endl;
		// std::cout << MAGENTA << "body without boundy : "<< WHIET  << std::endl;
		// std::cout << request.parse_body() << std::endl;
		// std::cout << MAGENTA << "body headers : "<< WHIET  << std::endl;
		// print_map(request.GetcontentdisPositionMap());
		std::cout <<  MAGENTA << "the two vals "<< WHIET  << std::endl;
		std::cout << "name : " << request.getNameValue() << " ; " << "filename : " << request.getFilenameValue() << std::endl;
		std::cout << "content type : " << request.getcontent_type_valbody() << std::endl;
		std::cout << MAGENTA << "passed body : " << WHIET << std::endl;
		std::cout << request.getparssed_body() << std::endl;
		std::cout << MAGENTA << "correct size : " << WHIET << std::endl;
		std::cout << (request.get_correct_size() ? "true" : "false") << std::endl;
		// std::cout << MAGENTA << "full cgi path : " << WHIET << std::endl;
		std::cout << "-----------------------------" << std::endl;
		// std::cout << "headers : " << std::endl;
		// else no cookies should be sent
		ParssedRequest req = _connections[ClientFd].getParsedRequest();
		ServerConfig confg = _connections[ClientFd].getServerConfig();
		// std::cout << confg.getHost() << ":" << std::endl;
		_connections[ClientFd].buildResponse(ClientFd, req, confg);
		// std::cout << YELLOW << currentTime() << GREEN << " [INFO] "
		//		<< "Request Is complete" << WHIET << std::endl;
		// std::cout << RED << "			------------------------------------			" << std::endl;
	}
	// else
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
