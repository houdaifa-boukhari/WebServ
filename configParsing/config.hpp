/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:42:31 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/08 23:31:44 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>
#include "serverConfig.hpp"

class Config
{
private:
	std::vector<ServerConfig> _servers;

public:
	Config();
	~Config();

};
#endif