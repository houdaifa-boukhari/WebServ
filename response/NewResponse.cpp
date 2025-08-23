/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:03:46 by yel-moun          #+#    #+#             */
/*   Updated: 2025/08/22 22:51:06 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NewResponse.hpp"

NewResponse::NewResponse()
{
	this->_sendStatus = SEND_NOT_STARTED;
	this->_clientFd = -1;
	this->_file_fd = -1;
	this->_config = ServerConfig();
	this->_request = ParssedRequest();
	this->_statusCode = 200;
	this->_byteSent = 0;
	this->_totalFileSize = 0;
	this->_httpVersion = "HTTP/1.1";
	this->_response = "";
	this->_body = "";
	this->_response_headers = std::map<std::string, std::string>();
	this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
	this->_response_headers["Connection"] = "Keep-Alive";
	this->_response_headers["Server"] = "WebServ/1.0";
}

NewResponse::NewResponse(int clientFd, ServerConfig &_config, ParssedRequest &request)
{
	this->_sendStatus = SEND_NOT_STARTED;
	this->_file_fd = -1;
	this->_config = _config;
	this->_request = request;
	this->_clientFd = clientFd;
	this->_statusCode = 200;
	this->_httpVersion = "HTTP/1.1";
	this->_response = "";
	this->_body = "";
	this->_response_headers = std::map<std::string, std::string>();
	this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
	this->_response_headers["Connection"] = "Keep-Alive";
	this->_response_headers["Server"] = "WebServ/1.0";
}

NewResponse::~NewResponse()
{
	if (this->_file_fd != -1)
	{
		close(this->_file_fd);
	}
}

void NewResponse::sendSimpleErrorResponse(int clientFd, int statusCode, const ServerConfig _config)
{
	std::string statusMessage = getStatusMessage(statusCode);
	std::string body = getErrorPage(statusCode, _config);
	std::string response =
		"HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n"
																		 "Content-Type: text/html; charset=UTF-8\r\n"
																		 "Content-Length: " +
		std::to_string(body.size()) + "\r\n"
									  "Connection: close\r\n"
									  "\r\n" +
		body;

	if (send(clientFd, response.c_str(), response.size(), 0) < 0)
	{
		std::cerr << "Unable to send response to client : " << clientFd << std::endl;
	}
}

void NewResponse::setClientFd(int clientFd)
{
	this->_clientFd = clientFd;
}

void NewResponse::setRequest(ParssedRequest &request)
{
	this->_request = request;
}

void NewResponse::setServerConfig(ServerConfig &config)
{
	this->_config = config;
}

SendStatus
NewResponse::getSendStatus() const
{
	return this->_sendStatus;
}

int NewResponse::getClientFd() const
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

std::string NewResponse::urlDecode(const std::string &str)
{
	std::string decoded;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '%' && i + 2 < str.length())
		{
			int value;
			std::istringstream is(str.substr(i + 1, 2));
			if (is >> std::hex >> value)
			{
				decoded += static_cast<char>(value);
				i += 2;
			}
			else
			{
				decoded += str[i];
			}
		}
		else if (str[i] == '+')
		{
			decoded += ' ';
		}
		else
		{
			decoded += str[i];
		}
	}
	return decoded;
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

std::string NewResponse::getCreatedResponseBody()
{
	std::string DefaultResponseBody = "<html><body><h1>Resource Created Successfully</h1></body></html>";
	std::string filePath = "./pages/success/201.html";
	if (fileExists(filePath))
	{
		std::string fileContent = getFileContent(filePath);
		if (!fileContent.empty())
			return fileContent;
		else
			return DefaultResponseBody;
	}
	return DefaultResponseBody;
}
std::string
NewResponse::getErrorPage(int statusCode, ServerConfig _config)
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
	LocationConfig *bestMatch = NULL;
	size_t bestLen = 0;
	bool isWildcardMatch = false;

	for (size_t i = 0; i < locations.size(); i++)
	{
		const std::string &locName = locations[i].getName();

		if (locName.size() > 1 && locName[0] == '*' && locName[1] == '.')
		{
			std::string extension = locName.substr(1); // Remove the '*'
			if (path.size() >= extension.size() &&
				path.substr(path.size() - extension.size()) == extension)
			{
				if (!bestMatch || !isWildcardMatch || extension.size() > bestLen)
				{
					bestLen = extension.size();
					bestMatch = &locations[i];
					isWildcardMatch = true;
				}
			}
		}
		else if (path.compare(0, locName.size(), locName) == 0 &&
				 (path.size() == locName.size() || path[locName.size()] == '/' || locName[locName.size() - 1] == '/'))
		{
			if (!isWildcardMatch && locName.size() > bestLen)
			{
				bestLen = locName.size();
				bestMatch = &locations[i];
			}
		}
	}

	if (bestMatch)
	{
		this->_matchedLocation = *bestMatch;
		return true;
	}
	return false;
}

void NewResponse::sendResponseToClient()
{
	this->_response_headers["Set-Cookie"] = this->_request.get_cookies_response();
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
		this->_sendStatus = SEND_ERROR;
		return;
	}
	else
	{
		this->_sendStatus = SEND_COMPLETED;
	}
}

void NewResponse::sendOnlyHeaders()
{
	this->_response_headers["Set-Cookie"] = this->_request.get_cookies_response();
	_response = _httpVersion + " " + std::to_string(_statusCode) + " " + this->getStatusMessage(_statusCode) + "\r\n";
	_response += "Date: " + responseUtils::getCurrentDate() + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _response_headers.begin(); it != _response_headers.end(); ++it)
	{
		_response += it->first + ": " + it->second + "\r\n";
	}
	_response += "\r\n";
	if (send(this->_clientFd, _response.c_str(), _response.size(), 0) < 0)
	{
		this->_sendStatus = SEND_ERROR;
		return;
	}
}

void NewResponse::generateResponse()
{
	if (_request.getMethod() == "GET")
		return handleGetRequest();
	else if (_request.getMethod() == "POST")
		return handlePostRequest();
	else if (_request.getMethod() == "DELETE")
		return handleDeleteRequest();
	else
	{
		this->_statusCode = 405;
		this->_response = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
}

void NewResponse::handleGetRequest()
{
	if (!findMatchingLocation(_request.get_path_without_params()))
	{
		this->_statusCode = 404;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (!isMethodAllowed(_request.getMethod(), _matchedLocation))
	{
		std::cout << "Method not allowed for path: " << _request.getPath() << std::endl;
		this->_statusCode = 405;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (_matchedLocation.getRoot().empty())
	{
		std::cout << "Root is empty for location: " << _matchedLocation.getName() << std::endl;
		this->_statusCode = 500;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (_matchedLocation.isRedirection())
	{
		std::cout << "Redirection found for path: " << _request.getPath() << std::endl;
		this->_statusCode = _matchedLocation.getRedirectionCode();
		this->_response_headers["Location"] = _matchedLocation.getRedirectionTo();
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (_request.is_cgi())
	{
		if (this->_request.get_status_code() >= 200 && this->_request.get_status_code() <= 308)
		{

			std::string cgi_response = _request.get_cgi_output();

			if (send(this->_clientFd, _request.get_cgi_output().c_str(), _request.get_cgi_output().length(), 0) < 0)
			{
				this->_sendStatus = SEND_ERROR;
				return;
			}
			else
			{
				this->_sendStatus = SEND_COMPLETED;
			}
		}
		else
		{
			this->_body = getErrorPage(this->_request.get_status_code(), this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
		}
		return;
	}
	else
	{
		std::string normalizedPath = normalizePath(this->urlDecode(_request.get_path_without_params()));
		std::string locationName = _matchedLocation.getName();
		std::string relativePath = normalizedPath.substr(locationName.size());
		if (!relativePath.empty() && relativePath[0] == '/')
			relativePath = relativePath.substr(1);
		std::string fullPath = joinPath(_matchedLocation.getRoot(), relativePath);
		if (isDirectory(fullPath))
			return handleDirectoryRequest(fullPath);
		else if (fileExists(fullPath))
			return prepareFileResponse(fullPath);
		else
		{
			std::cout << "File not found for path: " << fullPath << std::endl;
			this->_statusCode = 404;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}
	}
}

void NewResponse::handlePostRequest()
{
	std::cout << "starting POST request handling" << std::endl;
	if (!findMatchingLocation(_request.get_path_without_params()))
	{
		this->_statusCode = 404;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (!isMethodAllowed(_request.getMethod(), _matchedLocation))
	{
		this->_statusCode = 405;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (_matchedLocation.getRoot().empty())
	{
		this->_statusCode = 500;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}

	if (_request.is_cgi())
	{
		if (this->_request.get_status_code() >= 200 && this->_request.get_status_code() <= 308)
		{

			std::string cgi_response = _request.get_cgi_output();

			if (send(this->_clientFd, _request.get_cgi_output().c_str(), _request.get_cgi_output().length(), 0) < 0)
			{
				this->_sendStatus = SEND_ERROR;
				return;
			}
			else
			{
				this->_sendStatus = SEND_COMPLETED;
			}
		}
		else
		{
			this->_body = getErrorPage(this->_request.get_status_code(), this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
		}
		return;
	}
	else
	{
		std::string filename = _request.getFilenameValue();
		if (filename.empty())
			filename = "upload.txt";
		std::string uploadPath = joinPath(_matchedLocation.getRoot(), filename);
		std::ofstream out(uploadPath.c_str(), std::ios::binary);

		if (!out.is_open())
		{
			this->_statusCode = 500;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}

		if (!_request.get_boundary().empty())
			out << _request.getparssed_body();
		else
			out << _request.getBody();

		out.close();

		this->_statusCode = 201;
		this->_body =
			"<!DOCTYPE html>\n"
			"<html lang=\"en\">\n"
			"<head>\n"
			"  <meta charset=\"UTF-8\">\n"
			"  <title>Upload Successful</title>\n"
			"  <style>\n"
			"    body { font-family: Arial, sans-serif; background: #f9f9f9; text-align: center; margin-top: 60px; }\n"
			"    .success { color: #388e3c; font-size: 2em; margin-bottom: 20px; }\n"
			"    .info { color: #555; }\n"
			"    a { color: #1976d2; text-decoration: none; }\n"
			"    a:hover { text-decoration: underline; }\n"
			"  </style>\n"
			"</head>\n"
			"<body>\n"
			"  <div class=\"success\">&#10003; File uploaded successfully!</div>\n"
			"  <div class=\"info\">Your file has been received by the server.</div>\n"
			"  <p><a href=\"/\">Return to Home</a></p>\n"
			"  <hr>\n"
			"  <p><em>WebServ/1.0</em></p>\n"
			"</body>\n"
			"</html>";
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
}

void NewResponse::handleDeleteRequest()
{
	std::cout << "starting DELETE request handling" << std::endl;

	if (!findMatchingLocation(_request.get_path_without_params()))
	{
		std::cout << "No matching location found for path: " << _request.getPath() << std::endl;
		this->_statusCode = 404;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	if (!isMethodAllowed(_request.getMethod(), _matchedLocation))
	{
		std::cout << "Method not allowed for path: " << _request.getPath() << std::endl;
		this->_statusCode = 405;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}

	if (_matchedLocation.getRoot().empty())
	{
		std::cout << "Root is empty for location: " << _matchedLocation.getName() << std::endl;
		this->_statusCode = 500;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}

	if (_request.is_cgi())
	{
		if (this->_request.get_status_code() >= 200 && this->_request.get_status_code() <= 308)
		{
			std::string cgi_response = _request.get_cgi_output();
			if (send(this->_clientFd, _request.get_cgi_output().c_str(), _request.get_cgi_output().length(), 0) < 0)
			{
				this->_sendStatus = SEND_ERROR;
				return;
			}
			else
			{
				this->_sendStatus = SEND_COMPLETED;
			}
		}
		else
		{
			this->_body = getErrorPage(this->_request.get_status_code(), this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
		}
		return;
	}
	else
	{
		std::map<std::string, std::string> params = _request.get_params();
		std::string fileToDelete;

		if (params.find("path") != params.end())
		{
			fileToDelete = params["path"];
			std::cout << "File to delete from query parameter: " << fileToDelete << std::endl;
		}
		else
		{
			std::cout << "No 'path' parameter found in query string" << std::endl;
			this->_statusCode = 400;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}

		fileToDelete = urlDecode(fileToDelete);

		fileToDelete = normalizePath(fileToDelete);

		std::string fullPath;
		if (fileToDelete[0] == '/')
		{
			fullPath = joinPath(_matchedLocation.getRoot(), fileToDelete.substr(1));
		}
		else
		{
			fullPath = joinPath(_matchedLocation.getRoot(), fileToDelete);
		}

		std::cout << "Full path to delete: " << fullPath << std::endl;

		if (!fileExists(fullPath))
		{
			std::cout << "File not found for deletion: " << fullPath << std::endl;
			this->_statusCode = 404;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}

		if (isDirectory(fullPath))
		{
			std::cout << "Cannot delete directory: " << fullPath << std::endl;
			this->_statusCode = 403;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}

		if (access(fullPath.c_str(), W_OK) != 0)
		{
			std::cout << "No write permission for file: " << fullPath << std::endl;
			this->_statusCode = 403;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}

		if (remove(fullPath.c_str()) == 0)
		{
			std::cout << "Successfully deleted file: " << fullPath << std::endl;
			this->_statusCode = 200; // Changed from 204 to 200 to include response body
			this->_body =
				"<!DOCTYPE html>\n"
				"<html lang=\"en\">\n"
				"<head>\n"
				"  <meta charset=\"UTF-8\">\n"
				"  <title>File Deleted</title>\n"
				"  <style>\n"
				"    body { font-family: Arial, sans-serif; background: #f9f9f9; text-align: center; margin-top: 60px; }\n"
				"    .success { color: #388e3c; font-size: 2em; margin-bottom: 20px; }\n"
				"    .info { color: #555; }\n"
				"    a { color: #1976d2; text-decoration: none; }\n"
				"    a:hover { text-decoration: underline; }\n"
				"  </style>\n"
				"</head>\n"
				"<body>\n"
				"  <div class=\"success\">&#10003; File deleted successfully!</div>\n"
				"  <div class=\"info\">File: " +
				fileToDelete + "</div>\n"
							   "  <p><a href=\"/\">Return to Home</a></p>\n"
							   "  <hr>\n"
							   "  <p><em>WebServ/1.0</em></p>\n"
							   "</body>\n"
							   "</html>";
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}
		else
		{
			std::cout << "Failed to delete file: " << fullPath << " - Error: " << std::endl;
			this->_statusCode = 500;
			this->_body = getErrorPage(this->_statusCode, this->_config);
			this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
			sendResponseToClient();
			return;
		}
	}
}
void NewResponse::handleDirectoryRequest(std::string &dirPath)
{
	std::string defaultFile = _matchedLocation.getDefaultFile();
	if (!defaultFile.empty())
	{
		std::string defaultFilePath = joinPath(dirPath, defaultFile);
		if (fileExists(defaultFilePath))
			return prepareFileResponse(defaultFilePath);
	}

	std::vector<std::string> indexFiles = _matchedLocation.getIndex();
	for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it)
	{
		std::string indexPath = joinPath(dirPath, *it);
		if (fileExists(indexPath))
			return prepareFileResponse(indexPath);
	}
	if (_matchedLocation.isAutoIndex())
	{
		generateDirectoryListing(dirPath);
		return;
	}
	else
	{
		this->_statusCode = 403;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
}

void NewResponse::generateDirectoryListing(std::string &filePath)
{
	std::string html = "<!DOCTYPE html>\n<html>\n<head>\n<title>Directory Listing</title>\n";
	html += "<style>body{font-family:Arial,sans-serif;margin:40px;}a{text-decoration:none;color:#0066cc;}a:hover{text-decoration:underline;}</style>\n";
	html += "</head>\n<body>\n<h1>Directory Listing for " + _request.getPath() + "</h1>\n<hr>\n<ul>\n";

	DIR *dir = opendir(filePath.c_str());
	if (dir != NULL)
	{
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			std::string name = entry->d_name;
			if (name != ".")
			{
				std::string requestPath = _request.getPath();
				if (requestPath.back() != '/')
					requestPath += "/";

				html += "<li><a href=\"" + requestPath + name;
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

void NewResponse::prepareFileResponse(const std::string &filePath)
{
	this->_file_fd = open(filePath.c_str(), O_RDONLY);
	if (this->_file_fd < 0)
	{
		this->_statusCode = 403;
		this->_body = getErrorPage(this->_statusCode, this->_config);
		this->_response_headers["Content-Type"] = "text/html; charset=UTF-8";
		sendResponseToClient();
		return;
	}
	this->_totalFileSize = getFileSize(filePath);
	this->_statusCode = 200;
	this->_response_headers["Content-Type"] = getMimeType(filePath);
	this->_response_headers["Transfer-Encoding"] = "chunked";
	sendOnlyHeaders();
	if (this->_totalFileSize == 0)
	{
		this->_sendStatus = SEND_COMPLETED;
		close(this->_file_fd);
		this->_file_fd = -1;
		return;
	}
	else
	{
		this->_sendStatus = SEND_IN_PROGRESS;
		this->_byteSent = 0;
	}
}

size_t NewResponse::sendAll(int sockfd, const void *buf, size_t len)
{
	size_t total_sent = 0;
	while (total_sent < len)
	{
		ssize_t n = send(sockfd, (const char *)buf + total_sent, len - total_sent, 0);
		if (n == -1)
		{
			return -1;
		}
		total_sent += n;
	}
	return total_sent;
}

void NewResponse::sendNextChunk()
{
	if (this->_sendStatus == SEND_COMPLETED || this->_sendStatus == SEND_ERROR)
		return;

	if (this->_file_fd < 0)
	{
		this->_sendStatus = SEND_ERROR;
		return;
	}

	char buffer[CHUNK_SIZE];

	ssize_t bytes_read = read(this->_file_fd, buffer, CHUNK_SIZE);

	if (bytes_read < 0)
	{
		std::cout << "error in read " << std::endl;
		close(this->_file_fd);
		this->_file_fd = -1;
		this->_sendStatus = SEND_ERROR;
		return;
	}
	if (bytes_read == 0)
	{
		const char *last_chunk = "0\r\n\r\n";
		ssize_t sent = send(this->_clientFd, last_chunk, strlen(last_chunk), 0);
		if (sent < 0)
		{

			this->_sendStatus = SEND_ERROR;
		}
		else
		{
			this->_sendStatus = SEND_COMPLETED;
		}
		close(this->_file_fd);
		this->_file_fd = -1;
		return;
	}

	std::ostringstream chunk_stream;
	chunk_stream << std::hex << bytes_read << "\r\n";
	std::string chunk_header = chunk_stream.str();
	std::string chunk = chunk_header + std::string(buffer, bytes_read) + "\r\n";

	ssize_t bytes_sent_now = send(this->_clientFd, chunk.c_str(), chunk.size(), 0);
	if (bytes_sent_now < 0)
	{
		close(this->_file_fd);
		this->_file_fd = -1;
		this->_sendStatus = SEND_ERROR;
		return;
	}
	this->_byteSent += bytes_read;

	this->_sendStatus = SEND_IN_PROGRESS;
}
