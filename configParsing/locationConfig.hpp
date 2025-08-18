/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:41:47 by yel-moun          #+#    #+#             */
/*   Updated: 2025/08/18 13:29:24 by yel-moun         ###   ########.fr       */
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
	std::vector<std::string> _index;
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
	std::string _default_file;

public:
	LocationConfig();
	~LocationConfig();
	void setName(std::string name);
	void setRoot(std::string root);
	void addIndex(std::string index);
	void addAllowedMethod(std::string allowedMethod);
	void setAutoIndex(bool autoIndex);
	void setRedirection(bool has_redirection, int redirection_code, std::string redirection_to);
	void setCgi(bool has_cgi, std::string cgi_extension, std::string cgi_path);
	void setUpload(bool has_upload, std::string upload_path);
	void setDefaultFile(std::string file);
	std::string getDefaultFile() const;
	std::string getName() const;
	std::string getRoot() const;
	std::vector<std::string> getIndex() const;
	std::string getIndexPath() const;
	bool getAutoIndex() const;
	std::vector<std::string> getAllowedMethods() const;
	bool isAutoIndex() const;
	bool isRedirection() const;
	int getRedirectionCode() const;
	std::string getRedirectionTo() const;
	bool isCgi() const;
	std::string getCgiExtension() const;
	std::string getCgiPath() const;
	bool isUpload() const;
	std::string getUploadPath() const;
	void printLocation() const;
};

#endif