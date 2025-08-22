/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:56:59 by yel-moun          #+#    #+#             */
/*   Updated: 2025/08/22 22:48:44 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileParser.hpp"

FileParser::FileParser(int argc, char **argv)
{
	if (argc == 2)
	{
		_filename = argv[1];
		_serverConfiguration = ServerConfiguration();
		_is_default = false;
	}
	else if (argc == 1)
	{
		_filename = "";
		_serverConfiguration = ServerConfiguration();
		_is_default = true;
	}
	else
	{
		throw WrongFileException();
	}
}

bool FileParser::checkFileExtension()
{
	size_t dotPos = _filename.find_last_of(".");
	if (dotPos == std::string::npos)
		return false;
	size_t extPos = _filename.find(FILE_EXTENSION);
	if (extPos == std::string::npos)
		return false;
	if (extPos + 5 != _filename.length())
		return false;
	return true;
}

void FileParser::start()
{
	if (_is_default)
	{
		createDefaultConfig();
	}
	else
	{
		if (!this->checkFileExtension())
			throw WrongExtentionException();
		if (!this->checkFileExistence())
			throw FileDoesNotExistException();
		this->readConfigFile();
		if (!this->checkBrackets() || !this->checkEndOfLine())
			throw WrongFileFormatException();
		this->parseFile();
	}
}

void FileParser::createDefaultConfig()
{
	ServerConfig defaultServer;

	defaultServer.addPort(8080);
	defaultServer.setHost("127.0.0.1");
	defaultServer.addServerName("");
	defaultServer.setMaxBodySize("1M");

	defaultServer.addErrorPage("404", "./pages/errors/404.html");
	defaultServer.addErrorPage("500", "./pages/errors/500.html");

	LocationConfig defaultLocation;
	defaultLocation.setName("/");
	defaultLocation.setRoot("./");
	defaultLocation.setAutoIndex(false);
	defaultLocation.addIndex("index.html");
	defaultLocation.addAllowedMethod("GET");
	defaultLocation.addAllowedMethod("POST");
	defaultLocation.addAllowedMethod("DELETE");

	defaultServer.addLocation(defaultLocation);

	_serverConfiguration.addServer(defaultServer);
}

void FileParser::readConfigFile()
{
	std::string line;
	while (std::getline(this->_configFile, line))
	{
		if (line.empty())
			continue;
		if (ParsingUtils::isComment(line))
			continue;
		line = ParsingUtils::removeWhiteSpaces(line);
		line = ParsingUtils::cleanInLineComment(line);
		if (line.empty())
			continue;
		this->_configLines.push_back(line);
	}
}

bool FileParser::checkFileExistence()
{
	this->_configFile.open(_filename);
	if (!this->_configFile.good())
		return false;
	return true;
}

const ServerConfiguration &FileParser::getServerConfiguration() const
{
	return _serverConfiguration;
}

bool FileParser::checkBrackets()
{
	int brackets = 0;

	for (size_t i = 0; i < this->_configLines.size(); i++)
	{
		if (this->_configLines[i].find("{") != std::string::npos)
			brackets++;
		if (this->_configLines[i].find("}") != std::string::npos)
			brackets--;
	}
	return brackets == 0;
}

bool FileParser::checkEndOfLine()
{
	for (size_t i = 0; i < this->_configLines.size(); i++)
	{

		if (this->_configLines[i].find("server") != std::string::npos)
		{
			if (this->_configLines[i].length() == 6)
				continue;
			else if ((this->_configLines[i].find("{") != std::string::npos) && (this->_configLines[i].find("{") + 1) == this->_configLines[i].length())
				continue;
			else
			{
				if (this->_configLines[i].find(";") != std::string::npos)
					continue;
				else
					return (false);
			}
		}
		else if (this->_configLines[i].find("location") != std::string::npos)
		{
			if (this->_configLines[i].length() == 8)
				continue;
			else if ((this->_configLines[i].find("{") != std::string::npos) && (this->_configLines[i].find("{") + 1) == this->_configLines[i].length())
				continue;
			else
			{
				if (this->_configLines[i].find(";") != std::string::npos)
					continue;
				else
					return (false);
			}
		}
		else if (this->_configLines[i].find("}") != std::string::npos)
		{
			if (this->_configLines[i].length() == 1)
				continue;
			else
				return (false);
		}
		else if (this->_configLines[i].find("{") != std::string::npos)
		{
			if (this->_configLines[i].length() == 1)
				continue;
			else
			{
				if (this->_configLines[i].find(";") != std::string::npos)
					continue;
				else
					return (false);
			}
		}
		else
		{
			if (this->_configLines[i].find(";") != std::string::npos)
				continue;
			else
				return (false);
		}
	}
	return true;
}

void FileParser::parseFile()
{
	std::vector<std::string>::iterator it = this->_configLines.begin();
	std::vector<std::string>::iterator end = this->_configLines.end();

	while (it != end)
	{
		if (it->find("server") != std::string::npos)
			it = handleServerBlock(it);
		else
			it++;
	}
}
void FileParser::parseServerDirective(const std::string &directive, ServerConfig &serverConfig)
{
	if (directive.find("listen") != std::string::npos)
	{
		std::string portsString = directive.substr(directive.find("listen") + 6, directive.find(";", directive.find("listen") + 6) - (directive.find("listen") + 6));
		portsString = ParsingUtils::removeWhiteSpaces(portsString);
		std::vector<std::string> ports = ParsingUtils::splitString(portsString, ' ');
		for (size_t i = 0; i < ports.size(); i++)
		{
			std::string port = ParsingUtils::removeWhiteSpaces(ports[i]);
			int portNumber = ParsingUtils::convertToNumber(port);
			if (portNumber < 0 || portNumber > 65535)
				throw WrongFileContentException();
			serverConfig.addPort(portNumber);
		}
	}
	else if (directive.find("server_name") != std::string::npos)
	{
		std::string serverNamesStr = directive.substr(
			directive.find("server_name") + 12,
			directive.find(";", directive.find("server_name") + 12) - (directive.find("server_name") + 12));
		serverNamesStr = ParsingUtils::removeWhiteSpaces(serverNamesStr);

		std::vector<std::string> serverNames = ParsingUtils::splitString(serverNamesStr, ' ');

		for (size_t i = 0; i < serverNames.size(); i++)
		{
			std::string serverName = ParsingUtils::removeWhiteSpaces(serverNames[i]);
			serverConfig.addServerName(serverName);
		}
	}
	else if (directive.find("host") != std::string::npos)
	{
		std::string host = directive.substr(directive.find("host") + 5, directive.find(";", directive.find("host") + 5) - (directive.find("host") + 5));
		host = ParsingUtils::removeWhiteSpaces(host);
		serverConfig.setHost(host);
	}
	else if (directive.find("client_max_body_size") != std::string::npos)
	{
		std::string maxBodySize = directive.substr(directive.find("client_max_body_size") + 20, directive.find(";", directive.find("client_max_body_size") + 20) - (directive.find("client_max_body_size") + 20));
		maxBodySize = ParsingUtils::removeWhiteSpaces(maxBodySize);
		serverConfig.setMaxBodySize(maxBodySize);
	}
	else if (directive.find("error_page") != std::string::npos)
	{
		std::string errorPage = directive.substr(directive.find("error_page") + 11, directive.find(";", directive.find("error_page") + 11) - (directive.find("error_page") + 11));
		errorPage = ParsingUtils::removeWhiteSpaces(errorPage);
		std::vector<std::string> errorPages = ParsingUtils::splitString(errorPage, ' ');
		if (errorPages.size() != 2)
			throw WrongFileContentException();
		serverConfig.addErrorPage(errorPages[0], errorPages[1]);
	}
}
std::vector<std::string>::iterator FileParser::handleServerBlock(std::vector<std::string>::iterator &it)
{
	ServerConfig serverConfig;
	it++;
	while (it != _configLines.end() && *it != "}")
	{
		if (it->find("location") != std::string::npos)
		{
			it = handleLocationBlock(it, serverConfig);
			continue;
		}
		parseServerDirective(*it, serverConfig);
		it++;
	}
	_serverConfiguration.addServer(serverConfig);
	return it;
}

void FileParser::parseLocationDirective(const std::string &directive, LocationConfig &locationConfig)
{
	// std::cout << directive << std::endl;

	if (directive.find("root") != std::string::npos)
	{
		std::string root = directive.substr(directive.find("root") + 5, directive.find(";", directive.find("root") + 5) - (directive.find("root") + 5));
		root = ParsingUtils::removeWhiteSpaces(root);
		locationConfig.setRoot(root);
	}
	else if (directive.find("autoindex") != std::string::npos)
	{
		std::string autoIndex = directive.substr(directive.find("autoindex") + 10, directive.find(";", directive.find("autoindex") + 10) - (directive.find("autoindex") + 10));
		autoIndex = ParsingUtils::removeWhiteSpaces(autoIndex);
		if (autoIndex == "on")
			locationConfig.setAutoIndex(true);
		else if (autoIndex == "off")
			locationConfig.setAutoIndex(false);
		else
			throw WrongFileContentException();
	}
	else if (directive.find("default_file") != std::string::npos)
	{
		std::string defaultFile = directive.substr(directive.find("default_file") + 13, directive.find(";", directive.find("default_file") + 13) - (directive.find("default_file") + 13));
		defaultFile = ParsingUtils::removeWhiteSpaces(defaultFile);
		if (defaultFile[0] == ':')
			defaultFile = defaultFile.substr(1);
		defaultFile = ParsingUtils::removeWhiteSpaces(defaultFile);
		locationConfig.setDefaultFile(defaultFile);
	}
	else if (directive.find("index") != std::string::npos)
	{
		std::string indexString = directive.substr(directive.find("index") + 6, directive.find(";", directive.find("index") + 6) - (directive.find("index") + 6));
		indexString = ParsingUtils::removeWhiteSpaces(indexString);
		std::vector<std::string> indexFiles = ParsingUtils::splitString(indexString, ' ');
		for (size_t i = 0; i < indexFiles.size(); i++)
		{
			std::string indexFile = ParsingUtils::removeWhiteSpaces(indexFiles[i]);
			locationConfig.addIndex(indexFile);
		}
	}

	else if (directive.find("allowed_methods") != std::string::npos)
	{
		std::string allowedMethods = directive.substr(directive.find("allowed_methods") + 15, directive.find(";", directive.find("allow_methods") + 14) - (directive.find("allow_methods") + 14));
		if (!allowedMethods.empty() && allowedMethods[allowedMethods.size() - 1] == ';')
			allowedMethods.erase(allowedMethods.size() - 1);
		allowedMethods = ParsingUtils::removeWhiteSpaces(allowedMethods);
		std::vector<std::string> methods = ParsingUtils::splitString(allowedMethods, ' ');
		for (size_t i = 0; i < methods.size(); i++)
		{
			if (!methods[i].empty() && methods[i][methods[i].size() - 1] == ';')
				methods[i].erase(methods[i].size() - 1);
			locationConfig.addAllowedMethod(methods[i]);
		}
	}
	else if (directive.find("return") != std::string::npos)
	{
		std::string redirection = directive.substr(directive.find("return") + 6, directive.find(";", directive.find("return") + 6) - (directive.find("return") + 6));
		redirection = ParsingUtils::removeWhiteSpaces(redirection);
		std::vector<std::string> redirectionParts = ParsingUtils::splitString(redirection, ' ');
		if (redirectionParts.size() != 2)
			throw WrongFileContentException();
		int redirectionCode = ParsingUtils::convertToNumber(redirectionParts[0]);
		locationConfig.setRedirection(true, redirectionCode, redirectionParts[1]);
	}
	else if (directive.find("cgi") != std::string::npos)
	{
		std::string cgi = directive.substr(directive.find("cgi") + 4, directive.find(";", directive.find("cgi") + 4) - (directive.find("cgi") + 4));
		cgi = ParsingUtils::removeWhiteSpaces(cgi);
		std::vector<std::string> cgiParts = ParsingUtils::splitString(cgi, ' ');
		if (cgiParts.size() != 2)
			throw WrongFileContentException();
		locationConfig.setCgi(true, cgiParts[0], cgiParts[1]);
	}
	else if (directive.find("upload") != std::string::npos)
	{
		std::string upload = directive.substr(directive.find("upload") + 7, directive.find(";", directive.find("upload") + 7) - (directive.find("upload") + 7));
		upload = ParsingUtils::removeWhiteSpaces(upload);
		locationConfig.setUpload(true, upload);
	}
}

std::vector<std::string>::iterator FileParser::handleLocationBlock(std::vector<std::string>::iterator &it, ServerConfig &serverConfig)
{
	LocationConfig locationConfig;
	std::string locationName;

	// Extract location name from the current line
	if (it->find("location") != std::string::npos)
	{
		locationName = it->substr(it->find("location") + 9, it->find("{") - (it->find("location") + 9));
		locationName = ParsingUtils::removeWhiteSpaces(locationName);
		locationConfig.setName(locationName);
	}
	it++;
	while (it != _configLines.end())
	{
		if (it->find("}") != std::string::npos)
		{
			it++;
			break;
		}
		parseLocationDirective(*it, locationConfig);
		it++;
	}
	if (locationConfig.getAllowedMethods().empty())
	{
		locationConfig.addAllowedMethod("GET");
		locationConfig.addAllowedMethod("POST");
		locationConfig.addAllowedMethod("DELETE");
	}
	serverConfig.addLocation(locationConfig);
	return it;
}

const char *
FileParser::WrongExtentionException::what() const throw()
{
	return "Wrong file extention example: file.conf";
}

const char *FileParser::WrongFileFormatException::what() const throw()
{
	return "Wrong file format";
}

const char *FileParser::WrongFileException::what() const throw()
{
	return "Please provide a valid file as argument";
}

const char *FileParser::FileDoesNotExistException::what() const throw()
{
	return "File does not exist";
}

const char *FileParser::WrongFileContentException::what() const throw()
{
	return "Wrong file content";
}