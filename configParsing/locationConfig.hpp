/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:41:47 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/06 16:29:31 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <iostream>
#include <vector>
#include <map>

class LocationConfig
{
private:
	std::string _name;
	std::string _root;
	std::string _index;
	std::vector<std::string> _allowedMethods;
	bool _autoIndex;
	// redirection variables
	bool _has_redirection;
	int _redirection_code;
	std::string _redirection_to;

	// CGI variables
	bool _has_cgi;
	std::string _cgi_extension;
	std::string _cgi_path;

	// upload variables
	bool _has_upload;
	std::string _upload_path;

public:
	LocationConfig();
	~LocationConfig();
	void setName(std::string name);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void addAllowedMethod(std::string allowedMethod);
	void setAutoIndex(bool autoIndex);
	void setRedirection(bool has_redirection, int redirection_code, std::string redirection_to);
	void setCgi(bool has_cgi, std::string cgi_extension, std::string cgi_path);
	void setUpload(bool has_upload, std::string upload_path);
	std::string getName();
	std::string getRoot();
	std::string getIndex();
	std::vector<std::string> getAllowedMethods();
	bool isAutoIndex();
	bool isRedirection();
	int getRedirectionCode();
	std::string getRedirectionTo();
	bool isCgi();
	std::string getCgiExtension();
	std::string getCgiPath();
	bool isUpload();
	std::string getUploadPath();
	void printLocation();
};

#endif