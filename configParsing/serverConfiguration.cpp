/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:42:53 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/02 21:15:50 by yel-moun         ###   ########.fr       */
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

void ServerConfiguration::addServer(ServerConfig server)
{
	_servers.push_back(server);
}

std::vector<ServerConfig> ServerConfiguration::getServers()
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