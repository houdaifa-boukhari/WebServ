/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:40:51 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/02 20:50:39 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

#include "locationConfig.hpp"

class ServerConfig
{
private:
	std::vector<int> _ports;
	std::string _host;
	std::string _serverName;

	// limits
	std::string _max_Body_Size;

	// error pages
	std::map<std::string, std::string> _errorPages;

	// locations
	std::vector<LocationConfig> _locations;

public:
	ServerConfig();
	~ServerConfig();
	void addPort(int port);
	void setHost(std::string host);
	void setServerName(std::string serverName);
	void setMaxBodySize(std::string maxBodySize);
	void addErrorPage(std::string key, std::string value);
	void addLocation(LocationConfig location);
	std::vector<int> getPorts();
	std::string getHost();
	std::string getServerName();
	std::string getMaxBodySize();
	std::string getErrorPage(std::string key);
	std::map<std::string, std::string> getErrorPages();
	std::vector<LocationConfig> getLocations();
	void printServer();
};

#endif