/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:29:35 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/04 12:40:27 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"

ServerConfig::ServerConfig()
{
	_ports = std::vector<int>();
	_host = "";
	_serverName = "";
	_max_Body_Size = "";
	_errorPages = std::map<std::string, std::string>();
	_locations = std::vector<LocationConfig>();
}

ServerConfig::~ServerConfig()
{
}
void ServerConfig::addPort(int port)
{
	_ports.push_back(port);
}

void ServerConfig::setHost(std::string host)
{
	_host = host;
}

void ServerConfig::setServerName(std::string serverName)
{
	_serverName = serverName;
}
void ServerConfig::setMaxBodySize(std::string maxBodySize)
{
	_max_Body_Size = maxBodySize;
}
void ServerConfig::addErrorPage(std::string key, std::string value)
{
	_errorPages[key] = value;
}

void ServerConfig::addLocation(LocationConfig location)
{
	_locations.push_back(location);
}

std::vector<int> ServerConfig::getPorts()
{
	return _ports;
}

std::string ServerConfig::getHost()
{
	return _host;
}

std::string ServerConfig::getServerName()
{
	return _serverName;
}

std::string ServerConfig::getMaxBodySize()
{
	return _max_Body_Size;
}

std::string ServerConfig::getErrorPage(std::string key)
{
	std::map<std::string, std::string>::iterator it = _errorPages.find(key);
	if (it != _errorPages.end())
		return it->second;
	return "";
}

std::map<std::string, std::string> ServerConfig::getErrorPages()
{
	return _errorPages;
}

std::vector<LocationConfig> ServerConfig::getLocations()
{
	return _locations;
}

void ServerConfig::printServer()
{
	std::cout << "Server Name: " << _serverName << std::endl;
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Ports: ";
	for (size_t i = 0; i < _ports.size(); i++)
	{
		std::cout << _ports[i];
		if (i != _ports.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;
	std::cout << "Max Body Size: " << _max_Body_Size << std::endl;
	std::cout << "Error Pages: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
	{
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}
	std::cout << "Number of Locations: " << RED << _locations.size() << RESET<< std::endl;
	std::cout << "Locations: " << std::endl;
	for (size_t i = 0; i < _locations.size(); i++)
	{
		std::cout << BLUE << i + 1 << ": " << RESET << std::endl;
		_locations[i].printLocation();
	}
}