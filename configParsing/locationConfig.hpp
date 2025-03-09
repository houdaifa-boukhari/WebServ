/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:41:47 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/08 23:12:27 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <iostream>
#include <map>

class LocationConfig
{
private:
	std::string _path;
	std::map<std::string, std::string> _directives;

public:
	LocationConfig();
	~LocationConfig();
	void setPath(std::string path);
	std::string getPath();
	void addDirective(std::string key, std::string value);
	std::string getDirective(std::string key);
	bool hasDirective(const std::string &key) const;
};

#endif