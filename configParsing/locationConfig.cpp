/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:42:15 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/26 12:35:42 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locationConfig.hpp"

LocationConfig::LocationConfig()
{
	_allowedMethods = std::vector<std::string>();
	_name = "";
	_root = "";
	_index = std::vector<std::string>();
	_autoIndex = false;
	_has_redirection = false;
	_redirection_code = 0;
	_redirection_to = "";
	_has_cgi = false;
	_cgi_extension = "";
	_cgi_path = "";
	_has_upload = false;
}

LocationConfig::~LocationConfig()
{
}

void LocationConfig::setName(std::string name)
{
	_name = name;
}

void LocationConfig::setRoot(std::string root)
{
	_root = root;
}

void LocationConfig::addIndex(std::string index)
{
	_index.push_back(index);
}

void LocationConfig::addAllowedMethod(std::string allowedMethod)
{
	_allowedMethods.push_back(allowedMethod);
}

void LocationConfig::setAutoIndex(bool autoIndex)
{
	_autoIndex = autoIndex;
}

void LocationConfig::setRedirection(bool has_redirection, int redirection_code, std::string redirection_to)
{
	_has_redirection = has_redirection;
	_redirection_code = redirection_code;
	_redirection_to = redirection_to;
}

void LocationConfig::setCgi(bool has_cgi, std::string cgi_extension, std::string cgi_path)
{
	_has_cgi = has_cgi;
	_cgi_extension = cgi_extension;
	_cgi_path = cgi_path;
}

void LocationConfig::setUpload(bool has_upload, std::string upload_path)
{
	_has_upload = has_upload;
	_upload_path = upload_path;
}

std::string LocationConfig::getName()
{
	return _name;
}

std::string LocationConfig::getRoot()
{
	return _root;
}

std::vector<std::string> LocationConfig::getIndex()
{
	return _index;
}

bool LocationConfig::getAutoIndex()
{
	return _autoIndex;
}

std::vector<std::string> LocationConfig::getAllowedMethods()
{
	return _allowedMethods;
}

bool LocationConfig::isAutoIndex()
{
	return _autoIndex;
}

bool LocationConfig::isRedirection()
{
	return _has_redirection;
}

int LocationConfig::getRedirectionCode()
{
	return _redirection_code;
}

std::string LocationConfig::getRedirectionTo()
{
	return _redirection_to;
}

bool LocationConfig::isCgi()
{
	return _has_cgi;
}

std::string LocationConfig::getCgiExtension()
{
	return _cgi_extension;
}

std::string LocationConfig::getCgiPath()
{
	return _cgi_path;
}

bool LocationConfig::isUpload()
{
	return _has_upload;
}

std::string LocationConfig::getUploadPath()
{
	return _upload_path;
}

std::string LocationConfig::getIndexPath()
{
	if (_index.empty())
		return "";
	return "/" + _index[0];
}

void LocationConfig::printLocation()
{
	std::cout << "Location Name: " << _name << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Index : " << std::endl;
	for (size_t i = 0; i < _index.size(); i++)
	{
		std::cout << "  " << _index[i] << std::endl;
	}
	std::cout << "Allowed Methods: ";
	for (size_t i = 0; i < _allowedMethods.size(); i++)
	{
		std::cout << _allowedMethods[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Auto Index: " << (_autoIndex ? "true" : "false") << std::endl;
	if (_has_redirection)
	{
		std::cout << "Redirection Code: " << _redirection_code << std::endl;
		std::cout << "Redirection To: " << _redirection_to << std::endl;
	}
	if (_has_cgi)
	{
		std::cout << "CGI Extension: " << _cgi_extension << std::endl;
		std::cout << "CGI Path: " << _cgi_path << std::endl;
	}
	if (_has_upload)
	{
		std::cout << "Upload Path: " << _upload_path << std::endl;
	}
}