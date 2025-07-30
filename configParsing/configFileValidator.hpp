/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileValidator.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:56:59 by yel-moun          #+#    #+#             */
/*   Updated: 2025/07/29 13:12:56 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_VALIDATOR
#define CONFIG_FILE_VALIDATOR

#include <string>
#include <vector>
#include "serverConfig.hpp"

class ConfigFileValidator
{
public:
	ConfigFileValidator(const std::vector<ServerConfig> &servers);
	ConfigFileValidator(const ConfigFileValidator &other);
	ConfigFileValidator &operator=(const ConfigFileValidator &other);
	~ConfigFileValidator();
	void validate() const;
	class WrongServerConfig : public std::exception
	{
		virtual const char *what() const throw();
	};

private:
	const std::vector<ServerConfig> &_servers;

	bool validateServer(const ServerConfig &server) const;
	bool validateLocation(const LocationConfig &location) const;
	bool validatePorts(const ServerConfig &server) const;
	bool validateHost(const ServerConfig &server) const;
	bool validateErrorPages(const ServerConfig &server) const;
	bool validateAllowedMethods(const LocationConfig &location) const;
};

#endif