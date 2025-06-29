/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:10:16 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/28 16:01:38 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <dirent.h>
#include <ctime>

Response::Response()
{
	this->_headers = std::map<std::string, std::string>();
	this->_statusCode = 200;
	this->_body = "";
	this->_response = "";
	this->_httpVersion = "HTTP/1.1";
	this->_headers["Content-Type"] = "text/html";
	this->_headers["Server"] = "WebServ/1.0";
}

Response::~Response()
{
}
Response::Response(const Response &other)
{
	*this = other;
}

Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		this->_response = other._response;
		this->_statusCode = other._statusCode;
		this->_headers = other._headers;
		this->_body = other._body;
		this->_httpVersion = other._httpVersion;
	}
	return *this;
}

void Response::setStatusCode(int code)
{
	this->_statusCode = code;
}

void Response::setBody(const std::string &body)
{
	this->_body = body;
}

void Response::setHeader(const std::string &key, const std::string &value)
{
	this->_headers[key] = value;
}

// Getter methods
std::string Response::getResponse() const
{
	return this->_response;
}

int Response::getStatusCode() const
{
	return this->_statusCode;
}

std::string Response::getBody() const
{
	return this->_body;
}

std::map<std::string, std::string> Response::getHeaders() const
{
	return this->_headers;
}

std::string Response::getMimeType(const std::string &filePath)
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

std::string Response::getStatusMessage(int code)
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

bool Response::fileExists(const std::string &filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

std::string Response::readFile(const std::string &filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
		return "";

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content;
}

bool Response::isDirectory(const std::string &path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) != 0)
		return false;
	return S_ISDIR(pathStat.st_mode);
}

std::string Response::getFileSize(const std::string &filePath)
{
	std::ifstream file(filePath.c_str(), std::ifstream::ate | std::ifstream::binary);
	if (!file.is_open())
		return "0";
	return std::to_string(file.tellg());
}

std::string Response::buildHttpResponse()
{
	_response = _httpVersion + " " + std::to_string(_statusCode) + " " + this->getStatusMessage(_statusCode) + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		_response += it->first + ": " + it->second + "\r\n";
	}
	_response += "\r\n";
	_response += _body;

	return _response;
}

void Response::generateDefaultErrorPage(int statusCode)
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

	setStatusCode(statusCode);
	setHeader("Content-Type", "text/html");
	setHeader("Content-Length", std::to_string(errorBody.size()));
	setBody(errorBody);
}

void Response::generateCustomErrorPage(int statusCode, const std::string &errorPagePath)
{
	if (fileExists(errorPagePath))
	{
		try
		{
			std::string errorContent = readFile(errorPagePath);
			setStatusCode(statusCode);
			setHeader("Content-Type", getMimeType(errorPagePath));
			setHeader("Content-Length", std::to_string(errorContent.size()));
			setBody(errorContent);
			return;
		}
		catch (const std::exception &e)
		{
		}
	}
	this->generateDefaultErrorPage(statusCode);
}

std::string Response::normalizePath(const std::string &path)
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

std::string Response::joinPath(const std::string &root, const std::string &path)
{
	std::string result = root;
	std::string cleanPath = path;

	if (!result.empty() && result[result.length() - 1] == '/')
		result = result.substr(0, result.length() - 1);

	if (cleanPath.empty() || cleanPath[0] != '/')
		cleanPath = "/" + cleanPath;

	return result + cleanPath;
}

std::string Response::generateResponse(size_t clientFd, ParssedRequest &request, ServerConfig &serverConfig)
{
	try
	{
		this->_requestHeaders = request.getHeaders();
		std::string method = request.getMethod();
		std::string path = request.getPath();
		std::string requestedResource = normalizePath(path);

		if (method == "GET")
			return handleGetRequest(requestedResource, serverConfig);

		else if (method == "POST")
			return handlePostRequest(requestedResource, request, serverConfig);

		else if (method == "DELETE")
			return handleDeleteRequest(requestedResource, serverConfig);
		else
		{
			generateDefaultErrorPage(405);
			return buildHttpResponse();
		}
	}
	catch (const std::exception &e)
	{
		// todo : get the correct Error page not just the default one
		generateDefaultErrorPage(500);
		return buildHttpResponse();
	}
}

std::string Response::handleGetRequest(const std::string &path, ServerConfig &serverConfig)
{
	LocationConfig matchedLocation;
	bool locationFound = findMatchingLocation(path, serverConfig, matchedLocation);
	std::cout << "Searching for location for GET request: " << path << std::endl;
	if (!locationFound)
	{
		std::cout << "Location not found for GET request: " << path << std::endl;

		generateDefaultErrorPage(404);
		return buildHttpResponse();
	}
	else
	{
		std::cout << "Matched location for GET request: " << matchedLocation.getName() << std::endl;
	}
	if (matchedLocation.isRedirection())
	{
		setStatusCode(matchedLocation.getRedirectionCode());
		setHeader("Location", matchedLocation.getRedirectionTo());
		setHeader("Content-Length", "0");
		setBody("");
		return buildHttpResponse();
	}

	if (!isMethodAllowed("GET", matchedLocation))
	{
		generateDefaultErrorPage(405);
		return buildHttpResponse();
	}

	std::string fullPath = joinPath(matchedLocation.getRoot(), path);
	if (isDirectory(fullPath))
	{
		return handleDirectoryRequest(fullPath, matchedLocation);
	}
	else if (fileExists(fullPath))
	{
		return handleFileRequest(fullPath);
	}
	else
	{

		// Check if there's a custom error page for 404
		std::string errorPagePath = serverConfig.getErrorPage("404");
		if (!errorPagePath.empty())
		{
			generateCustomErrorPage(404, errorPagePath);
		}
		else
		{
			generateDefaultErrorPage(404);
		}
		return buildHttpResponse();
	}
}

std::string Response::handlePostRequest(const std::string &path, ParssedRequest &request, ServerConfig &serverConfig)
{
	// Find matching location
	LocationConfig matchedLocation;
	bool locationFound = findMatchingLocation(path, serverConfig, matchedLocation);

	if (!locationFound)
	{
		generateDefaultErrorPage(404);
		return buildHttpResponse();
	}

	// Check if method is allowed
	if (!isMethodAllowed("POST", matchedLocation))
	{
		generateDefaultErrorPage(405);
		return buildHttpResponse();
	}

	// Check content length against max body size
	std::map<std::string, std::string> headers = request.getHeaders();
	if (headers.find("Content-Length") != headers.end())
	{
		size_t contentLength = std::stoul(headers["Content-Length"]);
		if (contentLength > serverConfig.getMaxBodySizeBytes())
		{
			generateDefaultErrorPage(413); // Payload Too Large
			return buildHttpResponse();
		}
	}

	// // Handle CGI if configured
	// std::string cgiPath = matchedLocation.getCgi();
	// if (!cgiPath.empty())
	// {
	// 	return handleCgiRequest(path, request, matchedLocation);
	// }

	// // Handle file upload if it's an upload location
	// if (matchedLocation.getUploadEnable())
	// {
	// 	return handleFileUpload(request, matchedLocation);
	// }

	// Default POST handling
	setStatusCode(201);
	setHeader("Content-Type", "text/html");
	std::string responseBody = "<html><body><h1>201 Created</h1><p>Resource created successfully.</p></body></html>";
	setHeader("Content-Length", std::to_string(responseBody.size()));
	setBody(responseBody);
	return buildHttpResponse();
}

std::string Response::handleDeleteRequest(const std::string &path, ServerConfig &serverConfig)
{
	// Find matching location
	LocationConfig matchedLocation;
	bool locationFound = findMatchingLocation(path, serverConfig, matchedLocation);

	if (!locationFound)
	{
		generateDefaultErrorPage(404);
		return buildHttpResponse();
	}

	// Check if method is allowed
	if (!isMethodAllowed("DELETE", matchedLocation))
	{
		generateDefaultErrorPage(405);
		return buildHttpResponse();
	}

	std::string fullPath = joinPath(matchedLocation.getRoot(), path);

	if (fileExists(fullPath))
	{
		if (remove(fullPath.c_str()) == 0)
		{
			setStatusCode(204); // No Content
			setHeader("Content-Length", "0");
			setBody("");
			return buildHttpResponse();
		}
		else
		{
			generateDefaultErrorPage(500);
			return buildHttpResponse();
		}
	}
	else
	{
		generateDefaultErrorPage(404);
		return buildHttpResponse();
	}
}

std::string Response::handleDirectoryRequest(const std::string &dirPath, LocationConfig &location)
{
	// Check for index files
	std::vector<std::string> indexFiles = location.getIndex();
	for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it)
	{
		std::string indexPath = joinPath(dirPath, *it);
		if (fileExists(indexPath))
		{
			return handleFileRequest(indexPath);
		}
	}

	// Check if autoindex is enabled
	if (location.getAutoIndex())
	{
		return generateDirectoryListing(dirPath);
	}
	else
	{
		generateDefaultErrorPage(403); // Forbidden
		return buildHttpResponse();
	}
}

std::string Response::handleFileRequest(const std::string &filePath)
{
	// Get file size first
	struct stat fileStat;
	if (stat(filePath.c_str(), &fileStat) != 0)
	{
		generateDefaultErrorPage(404);
		return buildHttpResponse();
	}

	size_t fileSize = fileStat.st_size;
	std::string mimeType = getMimeType(filePath);

	std::cout << "-----------------------------------------------------------------" << std::endl;
	std::cout << "File size: " << fileSize << " bytes" << std::endl;
	std::cout << "MIME type: " << mimeType << std::endl;
	std::cout << "-----------------------------------------------------------------" << std::endl;

	const size_t MIN_RANGE_FILE_SIZE = 1024 * 1024;
	bool rangeSupport = false;
	std::map<std::string, std::string>::iterator rangeIt = _requestHeaders.find("Range");
	if (rangeIt != _requestHeaders.end())
		rangeSupport = true;
	if (rangeSupport)
		return handleRangeRequest(filePath, rangeIt->second, fileSize, mimeType);
	if (fileSize >= MIN_RANGE_FILE_SIZE)
	{
		return handleRangeRequest(filePath, "", fileSize, mimeType);
	}

	// For large files without range request, still read entire file (but add range support)
	std::string content = readFile(filePath);
	if (content.empty())
	{
		generateDefaultErrorPage(500);
		return buildHttpResponse();
	}

	setStatusCode(200);
	setHeader("Content-Type", mimeType);
	setHeader("Content-Length", std::to_string(content.size()));
	setHeader("Accept-Ranges", "bytes"); // Tell browser we support ranges
	setBody(content);
	return buildHttpResponse();
}

std::string Response::handleFileUpload(ParssedRequest &request, LocationConfig &location)
{
	std::string uploadPath = location.getUploadPath();
	std::string body = request.getBody();

	// Generate unique filename (simplified)
	std::time_t now = std::time(0);
	std::string filename = "upload_" + std::to_string(now) + ".txt";
	std::string fullPath = joinPath(uploadPath, filename);

	std::ofstream file(fullPath.c_str());
	if (file.is_open())
	{
		file << body;
		file.close();

		setStatusCode(201);
		setHeader("Content-Type", "text/html");
		std::string responseBody = "<html><body><h1>File Uploaded</h1><p>File uploaded successfully as " + filename + "</p></body></html>";
		setHeader("Content-Length", std::to_string(responseBody.size()));
		setBody(responseBody);
		return buildHttpResponse();
	}
	else
	{
		generateDefaultErrorPage(500);
		return buildHttpResponse();
	}
}

std::string Response::generateDirectoryListing(const std::string &dirPath)
{
	std::string html = "<!DOCTYPE html>\n<html>\n<head>\n<title>Directory Listing</title>\n";
	html += "<style>body{font-family:Arial,sans-serif;margin:40px;}a{text-decoration:none;color:#0066cc;}a:hover{text-decoration:underline;}</style>\n";
	html += "</head>\n<body>\n<h1>Directory Listing for " + dirPath + "</h1>\n<hr>\n<ul>\n";

	DIR *dir = opendir(dirPath.c_str());
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

	setStatusCode(200);
	setHeader("Content-Type", "text/html");
	setHeader("Content-Length", std::to_string(html.size()));
	setBody(html);
	return buildHttpResponse();
}

bool Response::findMatchingLocation(const std::string &path, ServerConfig &serverConfig, LocationConfig &matchedLocation)
{
	std::vector<LocationConfig> locations = serverConfig.getLocations();
	std::string bestMatch = "";
	bool found = false;

	for (std::vector<LocationConfig>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		std::string locationPath = it->getName();
		if (path.find(locationPath) == 0)
		{
			if (locationPath.length() > bestMatch.length())
			{
				bestMatch = locationPath;
				matchedLocation = *it;
				found = true;
			}
		}
	}

	return found;
}

bool Response::isMethodAllowed(const std::string &method, LocationConfig &location)
{
	std::vector<std::string> allowedMethods = location.getAllowedMethods();
	for (std::vector<std::string>::const_iterator it = allowedMethods.begin(); it != allowedMethods.end(); ++it)
	{
		if (*it == method)
		{
			return true;
		}
	}
	return false;
}

std::string Response::handleRangeRequest(const std::string &filePath,
										 const std::string &rangeHeader,
										 size_t fileSize,
										 const std::string &mimeType)
{
	std::cout << "Handling range request for file: " << filePath << std::endl;
	std::map<std::string, size_t> rangeHeaders = responseUtils::extractRangeHeaders(rangeHeader);
	size_t start = rangeHeaders["start"];
	size_t end = rangeHeaders["end"];
	std::cout << "Range header: " << rangeHeader << std::endl;

	// Validate and clamp range
	if (start > end || start >= fileSize)
	{
		// Invalid range, respond with 416
		setStatusCode(416);
		setHeader("Content-Range", "bytes */" + std::to_string(fileSize));
		setHeader("Content-Length", "0");
		setBody("");
		return buildHttpResponse();
	}
	if (end >= fileSize)
	{
		end = fileSize - 1;
	}
	size_t requestedSize = end - start + 1;

	// Handle empty file early
	if (fileSize == 0)
	{
		setStatusCode(200);
		setHeader("Content-Length", "0");
		setBody("");
		return buildHttpResponse();
	}

	std::cout << "Range request: start=" << start << ", end=" << end << std::endl;
	std::cout << "Requested size: " << requestedSize << " bytes" << std::endl;

	// Open file
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "File open failed: " << filePath << std::endl;
		generateDefaultErrorPage(404);
		return buildHttpResponse();
	}

	// Seek to start position
	file.seekg(static_cast<std::streamoff>(start));
	if (!file)
	{
		std::cerr << "File seek failed: " << start << std::endl;
		generateDefaultErrorPage(500);
		return buildHttpResponse();
	}

	// Read file chunk
	std::vector<char> buffer(requestedSize);
	file.read(buffer.data(), static_cast<std::streamsize>(requestedSize));
	size_t bytesRead = static_cast<size_t>(file.gcount());
	file.close();

	if (bytesRead == 0)
	{
		std::cerr << "File read failed: 0 bytes read" << std::endl;
		generateDefaultErrorPage(500);
		return buildHttpResponse();
	}

	size_t actualEnd = start + bytesRead - 1;

	// Set response headers
	if (start > 0 || actualEnd < fileSize - 1)
	{
		setStatusCode(206); // Partial Content
		setHeader("Content-Range", "bytes " +
									   std::to_string(start) + "-" +
									   std::to_string(actualEnd) + "/" +
									   std::to_string(fileSize));
	}
	else
	{
		setStatusCode(200); // Full content
	}

	setHeader("Content-Type", mimeType);
	setHeader("Content-Length", std::to_string(bytesRead));
	setHeader("Accept-Ranges", "bytes");
	setBody(std::string(buffer.data(), bytesRead));

	std::cout << "Served: " << start << "-" << actualEnd
			  << " (" << bytesRead << " bytes)" << std::endl;

	return buildHttpResponse();
}
