/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:29:35 by yel-moun          #+#    #+#             */
/*   Updated: 2025/08/22 22:48:33 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"

ServerConfig::ServerConfig()
{
	_ports = std::vector<int>();
	_host = "";
	_serverNames = std::vector<std::string>();
	_serverNames.push_back("localhost");
	_max_Body_Size = "";
	_max_Body_Size_Bytes = 1024 * 1024;
	_errorPages = std::map<std::string, std::string>();
	_locations = std::vector<LocationConfig>();
}

ServerConfig::ServerConfig(const ServerConfig &other)
{
	*this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		_ports = other._ports;
		_host = other._host;
		_serverNames = other._serverNames;
		_max_Body_Size = other._max_Body_Size;
		_max_Body_Size_Bytes = other._max_Body_Size_Bytes;
		_errorPages = other._errorPages;
		_locations = other._locations;
	}
	return *this;
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

void ServerConfig::addServerName(std::string serverName)
{
	_serverNames.push_back(serverName);
}

size_t ServerConfig::parseBodySizeToBytes(const std::string &sizeStr)
{
	if (sizeStr.empty())
		return 1024 * 1024;

	std::string cleanStr = sizeStr;
	cleanStr.erase(std::remove_if(cleanStr.begin(), cleanStr.end(), ::isspace), cleanStr.end());

	if (cleanStr.empty())
		return 1024 * 1024;

	size_t i = 0;
	while (i < cleanStr.length() && std::isdigit(cleanStr[i]))
		i++;

	if (i == 0)
		throw std::invalid_argument("Invalid body size format");

	long long number = std::atoll(cleanStr.substr(0, i).c_str());
	if (number < 0)
		throw std::invalid_argument("Body size cannot be negative");

	// Get the suffix (if any)
	std::string suffix = cleanStr.substr(i);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

	size_t multiplier = 1;
	if (suffix.empty() || suffix == "b")
		multiplier = 1;
	else if (suffix == "k" || suffix == "kb")
		multiplier = 1024;
	else if (suffix == "m" || suffix == "mb")
		multiplier = 1024 * 1024;
	else if (suffix == "g" || suffix == "gb")
		multiplier = 1024 * 1024 * 1024;
	else
		throw std::invalid_argument("Invalid body size suffix: " + suffix);
	return static_cast<size_t>(number * multiplier);
}

void ServerConfig::setMaxBodySize(std::string maxBodySize)
{
	_max_Body_Size = maxBodySize;
	_max_Body_Size_Bytes = parseBodySizeToBytes(maxBodySize);
}

size_t ServerConfig::getMaxBodySizeBytes() const
{
	return _max_Body_Size_Bytes;
}
void ServerConfig::addErrorPage(std::string key, std::string value)
{
	_errorPages[key] = value;
}

void ServerConfig::addLocation(LocationConfig location)
{
	_locations.push_back(location);
}

std::vector<int> ServerConfig::getPorts() const
{
	return _ports;
}

std::string ServerConfig::getHost() const
{
	return _host;
}

std::vector<std::string> ServerConfig::getServerNames() const
{
	return _serverNames;
}

std::string ServerConfig::getMaxBodySize() const
{
	return _max_Body_Size;
}

std::string ServerConfig::getErrorPage(std::string key) const
{
	std::map<std::string, std::string>::const_iterator it = _errorPages.find(key);
	if (it != _errorPages.end())
		return it->second;
	return "";
}

const std::map<std::string, std::string> &ServerConfig::getErrorPages() const
{
	return _errorPages;
}

std::vector<LocationConfig> ServerConfig::getLocations() const
{
	return _locations;
}

void ServerConfig::printServer()
{
	std::cout << "Server Names: ";
	for (size_t i = 0; i < _serverNames.size(); i++)
	{
		std::cout << _serverNames[i];
		if (i != _serverNames.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;
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
	std::cout << "Max Body Size in Bytes: " << _max_Body_Size_Bytes << std::endl;
	std::cout << "Error Pages: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
	{
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}

	std::cout << "Number of Locations: " << RED << _locations.size() << RESET << std::endl;
	std::cout << "Locations: " << std::endl;
	for (size_t i = 0; i < _locations.size(); i++)
	{
		std::cout << BLUE << i + 1 << ": " << RESET << std::endl;
		_locations[i].printLocation();
	}
}