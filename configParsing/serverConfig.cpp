/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:29:35 by yel-moun          #+#    #+#             */
/*   Updated: 2025/07/02 20:57:28 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"

ServerConfig::ServerConfig()
{
	_ports = std::vector<int>();
	_host = "";
	_serverName = "";
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
		_serverName = other._serverName;
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

void ServerConfig::setServerName(std::string serverName)
{
	_serverName = serverName;
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

	// if (number > (LLONG_MAX / multiplier))
	// 	throw std::overflow_error("Body size too large");

	return static_cast<size_t>(number * multiplier);
}

void ServerConfig::setMaxBodySize(std::string maxBodySize)
{
	_max_Body_Size = maxBodySize;
	_max_Body_Size_Bytes = parseBodySizeToBytes(maxBodySize);
}

size_t ServerConfig::getMaxBodySizeBytes()
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