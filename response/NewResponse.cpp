/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:03:46 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/28 17:50:14 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NewResponse.hpp"

NewResponse::NewResponse(size_t clientFd, ServerConfig &_config, ParssedRequest &request)
{
	this->_config = _config;
	this->_request = request;
	this->_clientFd = clientFd;
	this->_statusCode = 200;
	this->_httpVersion = "HTTP/1.1";
	this->_response = "";
	this->_body = "";
	this->_response_headers = std::map<std::string, std::string>();
	this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
	this->_response_headers["Connection"] = "close";
	this->_response_headers["Server"] = "WebServ/1.0";
}

NewResponse::~NewResponse()
{
}

size_t NewResponse::getClientFd() const
{
	return this->_clientFd;
}

std::string NewResponse::getResponse() const
{
	return this->_response;
}

int NewResponse::getStatusCode() const
{
	return this->_statusCode;
}

std::map<std::string, std::string> NewResponse::getResponseHeaders() const
{
	return this->_response_headers;
}

std::string NewResponse::getBody() const
{
	return this->_body;
}
std::string NewResponse::getHttpVersion() const
{
	return this->_httpVersion;
}

std::string NewResponse::getMimeType(const std::string &filePath)
{
	size_t pos = filePath.find_last_of(".");
	if (pos == std::string::npos)
		return "application/octet-stream";
	std::string extension = responseUtils::toLowerCase(filePath.substr(pos));

	if (extension == ".html" || extension == ".htm")
		return "text/html";
	if (extension == ".css")
		return "text/css";
	if (extension == ".js")
		return "application/javascript";
	if (extension == ".txt")
		return "text/plain";
	if (extension == ".xml")
		return "application/xml";
	if (extension == ".json")
		return "application/json";

	if (extension == ".png")
		return "image/png";
	if (extension == ".jpg" || extension == ".jpeg")
		return "image/jpeg";
	if (extension == ".gif")
		return "image/gif";
	if (extension == ".svg")
		return "image/svg+xml";
	if (extension == ".ico")
		return "image/x-icon";
	if (extension == ".webp")
		return "image/webp";

	if (extension == ".pdf")
		return "application/pdf";
	if (extension == ".zip")
		return "application/zip";
	if (extension == ".tar")
		return "application/x-tar";
	if (extension == ".gz")
		return "application/gzip";

	if (extension == ".mp4")
		return "video/mp4";
	if (extension == ".mp3")
		return "audio/mpeg";
	if (extension == ".webm")
		return "video/webm";
	if (extension == ".wav")
		return "audio/wav";

	if (extension == ".woff")
		return "font/woff";
	if (extension == ".woff2")
		return "font/woff2";
	if (extension == ".ttf")
		return "font/ttf";
	return "application/octet-stream";
}

std::string NewResponse::getStatusMessage(int code)
{
	switch (code)
	{
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 204:
		return "No Content";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 206:
		return "Partial Content";
	case 405:
		return "Method Not Allowed";
	case 413:
		return "Payload Too Large";
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 416:
		return "Range Not Satisfiable";
	default:
		return "Unknown";
	}
}

size_t NewResponse::getFileSize(const std::string &filePath)
{
	struct stat fileStat;
	if (stat(filePath.c_str(), &fileStat) == 0)
		return fileStat.st_size;
	return 0;
}

std::string NewResponse::getFileContent(const std::string &filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
		return "";

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content;
}

std::string NewResponse::getErrorPage(int statusCode)
{
	std::string errorTitle = std::to_string(statusCode) + " " + getStatusMessage(statusCode);
	std::string errorBody =
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
		"    <title>" +
		errorTitle + "</title>\n"
					 "    <style>\n"
					 "        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
					 "        h1 { color: #d32f2f; }\n"
					 "        p { color: #666; }\n"
					 "    </style>\n"
					 "</head>\n"
					 "<body>\n"
					 "    <h1>" +
		errorTitle + "</h1>\n"
					 "    <p>The requested resource could not be found or accessed.</p>\n"
					 "    <hr>\n"
					 "    <p><em>WebServ/1.0</em></p>\n"
					 "</body>\n"
					 "</html>";
	if (_config.getErrorPages().empty() || _config.getErrorPage(std::to_string(statusCode)) == "")
		return errorBody;
	else
	{
		std::string customErrorPagePath = _config.getErrorPage(std::to_string(statusCode));
		if (fileExists(customErrorPagePath))
		{
			std::string customErrorPageContent = getFileContent(customErrorPagePath);
			if (!customErrorPageContent.empty())
				return customErrorPageContent;
		}
		return errorBody;
	}
}

std::string NewResponse::normalizePath(const std::string &path)
{
	std::string normalized = path;

	size_t pos = 0;
	while ((pos = normalized.find("//", pos)) != std::string::npos)
	{
		normalized.replace(pos, 2, "/");
	}
	while ((pos = normalized.find("../")) != std::string::npos)
	{
		normalized.erase(pos, 3);
	}
	if (normalized.empty() || normalized[0] != '/')
		normalized = "/" + normalized;

	return normalized;
}

std::string NewResponse::joinPath(const std::string &root, const std::string &path)
{
	std::string result = root;
	std::string cleanPath = path;

	if (!result.empty() && result[result.length() - 1] == '/')
		result = result.substr(0, result.length() - 1);

	if (cleanPath.empty() || cleanPath[0] != '/')
		cleanPath = "/" + cleanPath;

	return result + cleanPath;
}

bool NewResponse::fileExists(const std::string &filePath)
{
	struct stat buffer;
	return (stat(filePath.c_str(), &buffer) == 0);
}

bool NewResponse::isDirectory(const std::string &path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) == 0)
		return S_ISDIR(pathStat.st_mode);
	return false;
}

bool NewResponse::isMethodAllowed(const std::string &method, LocationConfig &location)
{
	std::vector<std::string> allowedMethods = location.getAllowedMethods();
	return std::find(allowedMethods.begin(), allowedMethods.end(), method) != allowedMethods.end();
}

bool NewResponse::findMatchingLocation(const std::string &path)
{
	std::vector<LocationConfig> locations = _config.getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		if (locations[i].getName() == path)
		{
			this->_matchedLocation = locations[i];
			return true;
		}
	}
	return false;
}

void NewResponse::sendResponseToClient()
{
	this->_response_headers["Content-Length"] = std::to_string(this->_body.size());
	_response = _httpVersion + " " + std::to_string(_statusCode) + " " + this->getStatusMessage(_statusCode) + "\r\n";
	_response += "Date: " + responseUtils::getCurrentDate() + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _response_headers.begin(); it != _response_headers.end(); ++it)
	{
		_response += it->first + ": " + it->second + "\r\n";
	}
	_response += "\r\n";
	_response += _body;
	if (send(this->_clientFd, _response.c_str(), _response.size(), 0) < 0)
	{
		perror("send");
		close(this->_clientFd);
		return;
	}
}

void NewResponse::sendResponseLargeFile()
{
	this->_response_headers["Content-Length"] = std::to_string(this->_response.size());
	_response = _httpVersion + " " + std::to_string(_statusCode) + " " + this->getStatusMessage(_statusCode) + "\r\n";
	_response += "Date: " + responseUtils::getCurrentDate() + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _response_headers.begin(); it != _response_headers.end(); ++it)
	{
		_response += it->first + ": " + it->second + "\r\n";
	}
	_response += "\r\n";
	if (send(this->_clientFd, _response.c_str(), _response.size(), 0) < 0)
	{
		perror("send");
		close(this->_clientFd);
		return;
	}
}

void NewResponse::generateResponse()
{
	if (_request.getMethod() == "GET")
		return handleGetRequest();
	else if (_request.getMethod() == "POST")
	{
	}
	else if (_request.getMethod() == "DELETE")
	{
	}
	else
	{
		this->_statusCode = 405;
		this->_response = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
}

void NewResponse::handleGetRequest()
{
	std::cout << "starting GET request handling" << std::endl;
	if (!findMatchingLocation(_request.getPath()))
	{
		std::cout << "No matching location found for path: " << _request.getPath() << std::endl;
		this->_statusCode = 404;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (!isMethodAllowed(_request.getMethod(), _matchedLocation))
	{
		std::cout << "Method not allowed for path: " << _request.getPath() << std::endl;
		this->_statusCode = 405;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (_matchedLocation.getRoot().empty())
	{
		std::cout << "Root is empty for location: " << _matchedLocation.getName() << std::endl;
		this->_statusCode = 500;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (_matchedLocation.isRedirection())
	{
		std::cout << "Redirection found for path: " << _request.getPath() << std::endl;
		this->_statusCode = _matchedLocation.getRedirectionCode();
		this->_response_headers["Location"] = _matchedLocation.getRedirectionTo();
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	std::string path = normalizePath(_request.getPath());
	std::string fullPath = joinPath(_matchedLocation.getRoot(), path);
	if (isDirectory(fullPath))
		return handleDirectoryRequest(fullPath);
	else if (fileExists(fullPath))
		return handleFileRequest(fullPath);
	else
	{
		std::cout << "File not found for path: " << fullPath << std::endl;
		this->_statusCode = 404;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
}

void NewResponse::handlePostRequest()
{
	// Implementation for handling POST requests
}

void NewResponse::handleDeleteRequest()
{
	// Implementation for handling DELETE requests
}

void NewResponse::handleDirectoryRequest(std::string &dirPath)
{
	std::vector<std::string> indexFiles = _matchedLocation.getIndex();
	for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it)
	{
		std::string indexPath = joinPath(dirPath, *it);
		if (fileExists(indexPath))
			return handleFileRequest(indexPath);
	}
	if (_matchedLocation.isAutoIndex())
	{
		generateDirectoryListing(dirPath);
		return;
	}
	else
	{
		this->_statusCode = 403;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
}

void NewResponse::generateDirectoryListing(std::string &filePath)
{
	std::string html = "<!DOCTYPE html>\n<html>\n<head>\n<title>Directory Listing</title>\n";
	html += "<style>body{font-family:Arial,sans-serif;margin:40px;}a{text-decoration:none;color:#0066cc;}a:hover{text-decoration:underline;}</style>\n";
	html += "</head>\n<body>\n<h1>Directory Listing for " + filePath + "</h1>\n<hr>\n<ul>\n";

	DIR *dir = opendir(filePath.c_str());
	if (dir != NULL)
	{
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			std::string name = entry->d_name;
			if (name != ".")
			{
				html += "<li><a href=\"" + name;
				if (entry->d_type == DT_DIR)
					html += "/";
				html += "\">" + name;
				if (entry->d_type == DT_DIR)
					html += "/";
				html += "</a></li>\n";
			}
		}
		closedir(dir);
	}

	html += "</ul>\n<hr>\n<p><em>WebServ/1.0</em></p>\n</body>\n</html>";

	this->_statusCode = 200;
	this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
	this->_body = html;
	sendResponseToClient();
	return;
}

void NewResponse::handleFileRequest(std::string &filePath)
{
	const size_t LARGE_FILE_THRESHOLD = 1024 * 1024; // 1MB
	int fd = open(filePath.c_str(), O_RDONLY);
	if (fd < 0)
	{
		this->_statusCode = 404;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	size_t fileSize = getFileSize(filePath);
	close(fd);
	if (fileSize > LARGE_FILE_THRESHOLD)
		return handleLargeFileRequest(filePath);
	else
		return handleSmallFileRequest(filePath);
}

void NewResponse::handleSmallFileRequest(std::string &filePath)
{
	std::cout << "Handling small file request for: " << filePath << std::endl;

	std::ifstream file(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		this->_statusCode = 403;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	this->_statusCode = 200;
	this->_body = content;
	this->_response_headers["Content-Type"] = getMimeType(filePath);
	sendResponseToClient();
}

void NewResponse::handleLargeFileRequest(std::string &filePath)
{
	std::cout << "Handling large file request for: " << filePath << std::endl;

	std::ifstream file(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		this->_statusCode = 403;
		this->_body = getErrorPage(this->_statusCode);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}

	this->_statusCode = 200;
	this->_response_headers["Content-Type"] = getMimeType(filePath);
	this->_response_headers["Content-Length"] = std::to_string(getFileSize(filePath));
	this->_response_headers["Connection"] = "close";

	// Send headers first
	std::string headers = _httpVersion + " " + std::to_string(_statusCode) + " " + getStatusMessage(_statusCode) + "\r\n";
	headers += "Date: " + responseUtils::getCurrentDate() + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _response_headers.begin(); it != _response_headers.end(); ++it)
	{
		headers += it->first + ": " + it->second + "\r\n";
	}
	headers += "\r\n";

	if (send(this->_clientFd, headers.c_str(), headers.length(), MSG_NOSIGNAL) < 0)
	{
		perror("send headers");
		file.close();
		return;
	}

	// Send file content in chunks with simplified error handling
	const size_t bufferSize = 64;
	char buffer[bufferSize];
	size_t totalBytesSent = 0;
	size_t fileSize = getFileSize(filePath);

	while (file.read(buffer, bufferSize) || file.gcount() > 0)
	{
		size_t bytesToSend = file.gcount();
		size_t totalSent = 0;

		while (totalSent < bytesToSend)
		{
			ssize_t sent = send(this->_clientFd, buffer + totalSent, bytesToSend - totalSent, MSG_NOSIGNAL);
			if (sent < 0)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					// Socket buffer is full, wait for it to be ready
					fd_set writeSet;
					FD_ZERO(&writeSet);
					FD_SET(this->_clientFd, &writeSet);

					struct timeval timeout;
					timeout.tv_sec = 10; // Longer timeout
					timeout.tv_usec = 0;

					int ready = select(this->_clientFd + 1, NULL, &writeSet, NULL, &timeout);
					if (ready <= 0)
					{
						std::cout << "Client disconnected or timeout during large file transfer" << std::endl;
						file.close();
						return;
					}
					// Continue to retry send
					continue;
				}
				else if (errno == EPIPE || errno == ECONNRESET)
				{
					std::cout << "Client disconnected during file transfer" << std::endl;
					file.close();
					return;
				}
				else
				{
					perror("send body");
					file.close();
					return;
				}
			}
			else if (sent == 0)
			{
				std::cout << "Connection closed by client" << std::endl;
				file.close();
				return;
			}

			totalSent += sent;
			totalBytesSent += sent;
		}

		// Optional: Progress indicator for very large files
		if (fileSize > 0 && totalBytesSent % (1024 * 1024) == 0) // Every MB
		{
			std::cout << "Sent " << totalBytesSent << "/" << fileSize << " bytes" << std::endl;
		}
	}

	file.close();
	std::cout << "Large file sent successfully (" << totalBytesSent << " bytes)" << std::endl;
}