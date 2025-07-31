/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileValidator.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:56:59 by yel-moun          #+#    #+#             */
/*   Updated: 2025/07/31 17:11:01 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configFileValidator.hpp"

ConfigFileValidator::ConfigFileValidator(const std::vector<ServerConfig> &servers) : _servers(servers)
{
}

ConfigFileValidator::ConfigFileValidator(const ConfigFileValidator &other) : _servers(other._servers)
{
}

ConfigFileValidator &ConfigFileValidator::operator=(const ConfigFileValidator &other)
{
	if (this != &other)
	{
	}
	return *this;
}

ConfigFileValidator::~ConfigFileValidator()
{
}

void ConfigFileValidator::validate() const
{
	std::vector<ServerConfig>::const_iterator begin = this->_servers.begin();
	std::vector<ServerConfig>::const_iterator end = this->_servers.end();
	while (begin != end)
	{
		if (!validateServer(*begin))
			throw WrongServerConfig();
		++begin;
	}
}

bool ConfigFileValidator::validateServer(const ServerConfig &server) const
{

	if (!validatePorts(server))
		return false;

	if (!validateHost(server))
		return false;

	if (!validateErrorPages(server))
		return false;
	const std::vector<LocationConfig> &locations = server.getLocations();
	for (size_t i = 0; i < locations.size(); ++i)
	{
		if (!validateLocation(locations[i]))
			return false;
	}
	return true;
}

bool ConfigFileValidator::validatePorts(const ServerConfig &server) const
{
	const std::vector<int> &ports = server.getPorts();
	if (ports.empty())
		return false;
	for (size_t i = 0; i < ports.size(); ++i)
	{
		if (ports[i] <= 0 || ports[i] > 65535)
			return false;
	}
	return true;
}

bool ConfigFileValidator::validateHost(const ServerConfig &server) const
{
	const std::string &host = server.getHost();
	if (host.empty() || host.length() > 255)
		return false;
	return true;
}

bool ConfigFileValidator::validateErrorPages(const ServerConfig &server) const
{
	const std::map<std::string, std::string> &errorPages = server.getErrorPages();
	for (std::map<std::string, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it)
	{
		if (it->first.empty() || it->second.empty())
			return false;
	}
	return true;
}

bool ConfigFileValidator::validateAllowedMethods(const LocationConfig &location) const
{
	const std::vector<std::string> &methods = location.getAllowedMethods();
	static const std::string validMethods[] = {"GET", "POST", "DELETE"};
	for (size_t i = 0; i < methods.size(); ++i)
	{
		bool found = false;
		for (size_t j = 0; j < 3; ++j)
		{
			if (methods[i] == validMethods[j])
			{
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

bool ConfigFileValidator::validateLocation(const LocationConfig &location) const
{
	if (!location.isRedirection() && location.getRoot().empty())
		return false;
	if (!validateAllowedMethods(location))
		return false;
	return true;
}

const char *ConfigFileValidator::WrongServerConfig::what() const throw()
{
	return "Wrong Server Config";
}