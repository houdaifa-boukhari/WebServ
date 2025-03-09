/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:40:51 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/09 17:27:20 by yel-moun         ###   ########.fr       */
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
	std::vector<LocationConfig> _locations;
	std::map<std::string, std::string> _directives;
	std::map<std::string, std::string> _errorPages;

public:
	ServerConfig();
	~ServerConfig();
	void addLocation(LocationConfig location);
	std::vector<LocationConfig> getLocations();
	void addDirective(std::string key, std::string value);
	std::string getDirective(std::string key);
	bool hasDirective(const std::string &key) const;
};

#endif