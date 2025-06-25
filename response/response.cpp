/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:10:16 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/25 13:46:15 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()
{
	this->_headers = std::map<std::string, std::string>();
	this->_statusCode = 200;
	this->_body = "";
	this->_response = "";
	this->_httpVersion = "HTTP/1.1";
	this->_headers["Content-Type"] = "text/html";
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