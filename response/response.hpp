/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:58:01 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/28 16:15:46 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <map>
#include "responseUtils.hpp"
#include "../ParseRequest/Request.hpp"
#include "../configParsing/serverConfig.hpp"
#include "../configParsing/locationConfig.hpp"
#include <sys/socket.h>

class Response
{
private:
	std::string _response;
	int _statusCode;
	std::map<std::string, std::string> _headers;
	std::map<std::string, std::string> _requestHeaders;

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
	// HTTP request handlers
	std::string handleGetRequest(const std::string &path, ServerConfig &serverConfig);
	std::string handlePostRequest(const std::string &path, ParssedRequest &request, ServerConfig &serverConfig);
	std::string handleDeleteRequest(const std::string &path, ServerConfig &serverConfig);
	std::string handleDirectoryRequest(const std::string &dirPath, LocationConfig &location);
	std::string handleFileRequest(const std::string &filePath);
	std::string handleFileUpload(ParssedRequest &request, LocationConfig &location);
	std::string generateDirectoryListing(const std::string &dirPath);
	bool findMatchingLocation(const std::string &path, ServerConfig &serverConfig, LocationConfig &matchedLocation);
	bool isMethodAllowed(const std::string &method, LocationConfig &location);
	std::string handleRangeRequest(const std::string &filePath, const std::string &rangeHeader, size_t fileSize, const std::string &mimeType);

public:
	Response();
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	// operations
	std::string generateResponse(size_t clientFd, ParssedRequest &request, ServerConfig &serverConfig);

	// Getters
	std::string getResponse() const;
	int getStatusCode() const;
	std::string getBody() const;
	std::map<std::string, std::string> getHeaders() const;

	// Setter
	void setStatusCode(int code);
	void setHeader(const std::string &key, const std::string &value);
	void setBody(const std::string &body);
};

#endif