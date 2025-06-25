/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:58:01 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/25 13:46:00 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <map>
#include "responseUtils.hpp"

class Response
{
private:
	std::string _response;
	int _statusCode;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _httpVersion;
	bool fileExists(const std::string &filePath);
	bool isDirectory(const std::string &path);
	std::string readFile(const std::string &filePath);
	std::string getMimeType(const std::string &filePath);
	std::string getStatusMessage(int code);
	std::string getFileSize(const std::string &filePath);
	std::string buildHttpResponse();
	void generateDefaultErrorPage(int statusCode);
	void generateCustomErrorPage(int statusCode, const std::string &errorPagePath);
	std::string normalizePath(const std::string &path);
	std::string joinPath(const std::string &root, const std::string &path);

public:
	Response();
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	// operations
	// Setter
	void setStatusCode(int code);
	void setHeader(const std::string &key, const std::string &value);
	void setBody(const std::string &body);
};

#endif