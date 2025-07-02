/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:59:27 by yel-moun          #+#    #+#             */
/*   Updated: 2025/07/02 14:02:38 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEWRESPONSE_HPP
#define NEWRESPONSE_HPP

#include <iostream>
#include <fstream>
#include <map>
#include "responseUtils.hpp"
#include <sys/stat.h>
#include "../ParseRequest/Request.hpp"
#include "../configParsing/serverConfig.hpp"
#include "../configParsing/locationConfig.hpp"
#include <sys/socket.h>
#include <dirent.h>

#define CHUNK_SIZE 32768

enum SendStatus
{
	SEND_NOT_STARTED,
	SEND_COMPLETED,
	SEND_IN_PROGRESS,
	SEND_ERROR
};

class NewResponse
{
private:
	int _clientFd;
	int _file_fd;
	std::string _response;
	SendStatus _sendStatus;
	int _statusCode;
	std::string _body;
	std::string _httpVersion;
	std::map<std::string, std::string> _response_headers;
	ServerConfig _config;
	ParssedRequest _request;
	LocationConfig _matchedLocation;
	size_t _byteSent;
	size_t _totalFileSize;

	// Private Function declarations

	// Utility functions
	bool fileExists(const std::string &filePath);
	bool isDirectory(const std::string &path);
	bool isMethodAllowed(const std::string &method, LocationConfig &location);
	bool findMatchingLocation(const std::string &path);

	std::string getMimeType(const std::string &filePath);
	std::string getStatusMessage(int code);
	size_t getFileSize(const std::string &filePath);
	std::string getFileContent(const std::string &filePath);
	std::string getErrorPage(int statusCode);
	std::string normalizePath(const std::string &path);
	std::string joinPath(const std::string &root, const std::string &path);
	void prepareFileResponse(const std::string &filePath);

	// HTTP request handlers
	void sendResponseToClient();
	void sendOnlyHeaders();
	void handleGetRequest();
	void handlePostRequest();
	void handleDeleteRequest();
	void handleDirectoryRequest(std::string &dirPath);
	void generateDirectoryListing(std::string &filePath);

public:
	NewResponse();
	NewResponse(int clientFd, ServerConfig &_config, ParssedRequest &request);
	void generateResponse();
	void sendNextChunk();
	~NewResponse();

	// Getters
	int
	getClientFd() const;
	std::string getResponse() const;
	int getStatusCode() const;
	std::map<std::string, std::string> getResponseHeaders() const;
	std::string getBody() const;
	std::string getHttpVersion() const;
	SendStatus getSendStatus() const;
};

#endif