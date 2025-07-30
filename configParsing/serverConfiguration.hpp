/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:42:31 by yel-moun          #+#    #+#             */
/*   Updated: 2025/07/29 13:32:36 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIGURATION_HPP
#define SERVERCONFIGURATION_HPP

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

#include <iostream>
#include <vector>
#include "serverConfig.hpp"

class ServerConfiguration
{
private:
	std::vector<ServerConfig> _servers;

public:
	ServerConfiguration(const ServerConfiguration &other);
	ServerConfiguration &operator=(const ServerConfiguration &other);
	ServerConfiguration();
	~ServerConfiguration();
	void addServer(ServerConfig server);
	std::vector<ServerConfig> &getServers();
	void printConfig();
};
#endif