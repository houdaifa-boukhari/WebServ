/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:40:51 by yel-moun          #+#    #+#             */
/*   Updated: 2025/08/22 22:31:35 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

#include "locationConfig.hpp"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHIET "\033[0m"

class ServerConfig
{
private:
	std::vector<int> _ports;
	std::string _host;
	std::vector<std::string> _serverNames;

	// limits
	std::string _max_Body_Size;
	size_t _max_Body_Size_Bytes;

	// error pages
	std::map<std::string, std::string> _errorPages;

	// locations
	std::vector<LocationConfig> _locations;
	size_t parseBodySizeToBytes(const std::string &sizeStr);

public:
	ServerConfig();
	ServerConfig(const ServerConfig &other);
	ServerConfig &operator=(const ServerConfig &other);
	~ServerConfig();
	void addPort(int port);
	void setHost(std::string host);
	void addServerName(std::string serverName);
	void setMaxBodySize(std::string maxBodySize);
	void addErrorPage(std::string key, std::string value);
	void addLocation(LocationConfig location);
	std::vector<int> getPorts() const;
	std::string getHost() const;
	std::vector<std::string> getServerNames() const;
	std::string getMaxBodySize() const;
	size_t getMaxBodySizeBytes() const;
	std::string getErrorPage(std::string key) const;
	const std::map<std::string, std::string> &getErrorPages() const;
	std::vector<LocationConfig> getLocations() const;
	void printServer();
};

#endif