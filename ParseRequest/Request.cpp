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
    clear_params();
    std::string::size_type pos = path.find('?');
    if (pos != std::string::npos)
    {
        std::string query_string = path.substr(pos + 1);
        path = path.substr(0, pos);
        std::istringstream iss(query_string);
        std::string param;
        while (std::getline(iss, param, '&'))
        {
            std::size_t eq_pos = param.find('=');
            if (eq_pos != std::string::npos)
            {
                std::string key = param.substr(0, eq_pos);
                std::string value = param.substr(eq_pos + 1);
                _params[trim(key)] = trim(value);
            }
            else
            {
                // Handle case where param has key but no '=' (e.g., ?name1)
                _params[trim(param)] = "";
            }
        }
    }
}

void ParssedRequest::clear_params()
{
    _params.clear();
}

std::map<std::string, std::string> ParssedRequest::getParams()
{
    return (_params);
}

void ParssedRequest::is_cgi_path(const std::string& path) {
    std::string prefix = "/cgi-bin/";
    // _cgi = true;
    _cgi =  path.compare(0, prefix.size(), prefix) == 0 && path.size() >= prefix.size();
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
        // through error
        return;
    }

    std::cout << "Method: " << method << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "Version: " << version << std::endl;
}

void ParssedRequest::assign_pwithout_params()
{
    std::string::size_type pos = _path.find('?');
    if (pos != std::string::npos)
        _path_without_params = _path.substr(0, pos);
    else
        _path_without_params = _path;
}

void ParssedRequest::assign_cookies(std::string key_values)
{
    std::istringstream stream(key_values);
    std::string cookie_value;
    while (std::getline(stream, cookie_value, ';'))
    {
        std::string key;
        std::string value;
        std::istringstream one_cookie(cookie_value);
        if (std::getline(one_cookie, key , '=') && std::getline(one_cookie, value))
        {
            key = trim(key);
            value = trim(value);
            _cookies[key] = value;
        }
    }
}

bool ParssedRequest::get_cookie_avai()
{
    if (_cookies.find("bg-color") != _cookies.end()) {
        return true;
    }
    return false;
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
    _cookies.clear();
    while (std::getline(stream, line) && !line.empty() && line != "\r")
    {
        iss.clear();
        iss.str(line);
        if (std::getline(iss, key, ':') && std::getline(iss, value))
        {
            // maybe check is key has two words
            if (trim(key) == "Cookie")
                assign_cookies(trim(value));
            _headers[trim(key)] = trim(value);
        }
        else
        {
            std::cerr << "Invalid header format: " << line << std::endl;
            return;
        }
    }
    assignhost_port();
    is_cgi_path(_path);
    //     _cgi = true;
    // else
    //     _cgi = false;
    assign_params(_path);
    assign_pwithout_params();
    // assign_cookies();
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
    int final_size = _params.size() + 3 + 1;
    char **envp = new char *[final_size]; // add new cookies
    envp[final_size - 1] = NULL;
    int i = 0;
    while (begin != end)
    {
        key_val = begin->first + "=" + begin->second;
        envp[i] = new char[key_val.size() + 1];
        std::strcpy(envp[i], key_val.c_str());
        i++;
        begin++;
    }
    std::string key = "REQUEST_METHOD";
    key_val = key + "=" + _method;
    envp[i] = new char [key_val.size() + 1];
    std::strcpy(envp[i], key_val.c_str());
    i++;
    key = "PATH_INFO";
    key_val = key + "=" + _path_info;
    envp[i] = new char [key_val.size() + 1];
    std::strcpy(envp[i], key_val.c_str());
    i++;
    key = "SCRIPT_NAME";
    key_val = key + "=" + _script_name;
    envp[i] = new char [key_val.size() + 1];
    std::strcpy(envp[i], key_val.c_str());
    return envp;
}

std::string ParssedRequest::get_host()
{
    return _host;
}

std::string ParssedRequest::get_port()
{
    return _port;
}

void ParssedRequest::assignhost_port()
{
    std::map<std::string, std::string>::iterator begin = _headers.begin();
    std::map<std::string, std::string>::iterator end = _headers.end();

    while (begin != end)
    {
        if (begin->first == "Host")
        {
            std::istringstream host_port(begin->second);
            if (std::getline(host_port, _host, ':') && std::getline(host_port, _port))
                return;
            else
            {
                std::cout << "something went wrong with assignhost_port" << std::endl;
                return ;
            }
            return ;
        }
        begin++;
    }
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

bool ParssedRequest::is_cgi()
{
    return _cgi;
}

void ParssedRequest::assign_full_cgi_path() {
    std::string cgi_files = "cgi_files";
    const std::string prefix = "/cgi-bin/";
    if (_path_without_params.compare(0, prefix.size(), prefix) == 0) {
        std::string relative = _path_without_params.substr(prefix.size());  // remove "cgi-bin/"
        cgi_path = cgi_files + "/" + relative;
    } else {
        // Not a valid cgi-bin path
        cgi_path = "";
    }
    std::cout << cgi_path << std::endl;
    // std::cout << cgi_path <<
}

bool ParssedRequest::path_exists()
{
    return access(cgi_path.c_str(), F_OK) == 0;
}

std::string ParssedRequest::get_cgi_path()
{
    return cgi_path;
}

void ParssedRequest::assign_cgi_output(std::string cgi_outpt)
{
    this->cgi_output = cgi_outpt;
}

std::string ParssedRequest::get_cgi_output(void)
{
    return cgi_output;
}

void ParssedRequest::assign_cookies_response(std::string cookies_response)
{
    this->cookies_response = cookies_response;
}

std::string ParssedRequest::get_cookies_response(void)
{
    return this->cookies_response;
}
