/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:42:53 by yel-moun          #+#    #+#             */
/*   Updated: 2025/07/29 13:32:22 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfiguration.hpp"

ServerConfiguration::ServerConfiguration()
{
	_servers = std::vector<ServerConfig>();
}

ServerConfiguration::~ServerConfiguration()
{
}


ServerConfiguration::ServerConfiguration(const ServerConfiguration &other)
{
	_servers = other._servers;
}


ServerConfiguration &ServerConfiguration::operator=(const ServerConfiguration &other)
{
	if (this != &other)
	{
		_servers = other._servers;
	}
	return *this;
}

void ServerConfiguration::addServer(ServerConfig server)
{
	_servers.push_back(server);
}

std::vector<ServerConfig> &ServerConfiguration::getServers()
{
	return _servers;
}

void ServerConfiguration::printConfig()
{
	std::cout << GREEN << "Server Configuration:" << RESET << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Number of servers: " << _servers.size() << std::endl;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Server " << GREEN << i + 1 << RESET << ":" << std::endl;
		_servers[i].printServer();
		std::cout << "----------------------------------------" << std::endl;
	}
}