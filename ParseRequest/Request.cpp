#include "Request.hpp"

std::string trim(const std::string &str)
{
    std::string::const_iterator start = str.begin();
    while (start != str.end() && std::isspace(*start))
    {
        ++start;
    }

    std::string::const_reverse_iterator r_end = str.rbegin();
    while (r_end != str.rend() && std::isspace(*r_end))
    {
        ++r_end;
    }

    std::string::const_iterator end = r_end.base();
    return (start < end) ? std::string(start, end) : "";
}

ParssedRequest::ParssedRequest(std::string request) : _request(request)
{
}

ParssedRequest::ParssedRequest()
{
}

ParssedRequest::~ParssedRequest()
{
}

void ParssedRequest::printParams()
{
    std::map<std::string, std::string>::iterator it;
    for (it = _params.begin(); it != _params.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void ParssedRequest::assign_params(std::string path)
{
    std::string::size_type pos = path.find('?');
    if (pos != std::string::npos)
    {
        std::string query_string = path.substr(pos + 1);
        path = path.substr(0, pos);
        std::istringstream iss(query_string);
        std::string param;
        while (std::getline(iss, param, '&'))
        {
            std::string key = param.substr(0, param.find('='));
            std::string value = param.substr(param.find('=') + 1);
            _params[trim(key)] = trim(value);
        }
    }
}

std::map<std::string, std::string> ParssedRequest::getParams()
{
    return (_params);
}

void ParssedRequest::AssignRequestLine(std::string request)
{
    std::istringstream iss(request);
    std::string method, path, version;

    if (!(iss >> method >> path >> version))
    {
        std::cerr << "Invalid request line format.\n";
        return;
    }

    _method = method;
    _path = path;
    _version = version;
    std::string extra;
    if (iss >> extra)
    {
        std::cerr << "Too many parts in request line.\n";
        return;
    }

    std::cout << "Method: " << method << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "Version: " << version << std::endl;
}

void ParssedRequest::AssignHeadersLine(std::string request)
{
    std::istringstream stream(request);
    std::string line;

    // // Read the request line
    if (!std::getline(stream, line) || line.empty())
    {
        std::cerr << "Empty or invalid request line.\n";
        return;
    }
    // Remove trailing '\r' if exists
    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    std::istringstream iss(line);
    std::string method, path, version;
    if (!(iss >> _method >> _path >> _version))
    {
        std::cerr << "Invalid request line format.\n";
        return;
    }
    std::string extra;
    if (iss >> extra)
    {
        std::cerr << "Too many parts in request line.\n";
        return;
    }
    // std::cout << "request line: " << line << std::endl;
    // std::cout << "HEADERS" << std::endl;
    std::string key;
    std::string value;
    while (std::getline(stream, line) && !line.empty() && line != "\r")
    {
        iss.clear();
        iss.str(line);
        if (std::getline(iss, key, ':') && std::getline(iss, value))
        {
            // maybe check is key has two words
            _headers[trim(key)] = trim(value);
        }
        else
        {
            std::cerr << "Invalid header format: " << line << std::endl;
            return;
        }
    }
    assign_params(_path);
    std::ostringstream body_stream;
    body_stream << stream.rdbuf(); // Reads the rest of the stream
    _body = body_stream.str();
}

void printMap(std::map<std::string, std::string> myMap)
{
    std::map<std::string, std::string>::iterator it;
    for (it = myMap.begin(); it != myMap.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

std::string ParssedRequest::GetReqeustLIne()
{
    return (_method + " " + _path + " " + _version);
}

void ParssedRequest::AssignHeaders(std::string request)
{
    (void)request;
}

char **ParssedRequest::get_env()
{
    std::map<std::string, std::string>::iterator begin = _params.begin();
    std::map<std::string, std::string>::iterator end = _params.end();
    std::string key_val;
    char **envp = new char *[_params.size() + 1];
    envp[_params.size()] = NULL;
    int i = 0;
    while (begin != end)
    {
        key_val = begin->first + "=" + begin->second;
        envp[i] = new char[key_val.size() + 1];
        std::strcpy(envp[i], key_val.c_str());
        i++;
        begin++;
    }
    return envp;
}

std::map<std::string, std::string> ParssedRequest::getHeaders()
{
    return (_headers);
}

std::string ParssedRequest::getBody()
{
    return (_body);
}

std::string ParssedRequest::getMethod() const
{
    return _method;
}

std::string ParssedRequest::getPath() const
{
    return _path;
}

std::string ParssedRequest::getVersion() const
{
    return _version;
}

void ParssedRequest::AssignBody(std::string request)
{
    (void)request;
}
