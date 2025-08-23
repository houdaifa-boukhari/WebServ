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

std::map<std::string, std::string> ParssedRequest::get_params() const
{
    return _params;
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

void ParssedRequest::is_cgi_path(const std::string &path)
{
    std::string prefix = "/cgi-bin/";
    // _cgi = true;
    _cgi = path.compare(0, prefix.size(), prefix) == 0 && path.size() >= prefix.size();
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
    std::string cookie_pair;

    while (std::getline(stream, cookie_pair, ';'))
    {
        // Trim whitespace from the entire cookie pair first
        cookie_pair = trim(cookie_pair);
        if (cookie_pair.empty())
            continue;

        size_t equals_pos = cookie_pair.find('=');

        // Case 1: "key=value" format
        if (equals_pos != std::string::npos)
        {
            std::string key = cookie_pair.substr(0, equals_pos);
            std::string value = cookie_pair.substr(equals_pos + 1);
            key = trim(key);
            value = trim(value);

            if (!key.empty())
            { // Only insert valid keys
                _cookies[key] = value;
            }
        }
        // Case 2: Malformed cookie (no '='), treat as empty value
        else if (!cookie_pair.empty())
        {
            std::string key = trim(cookie_pair);
            if (!key.empty())
            {
                _cookies[key] = ""; // Explicit empty value
            }
        }
    }
}

int ParssedRequest::get_cookie_avai()
{
    if (_cookies.find("bg-color") == _cookies.end() || _cookies.find("session-id") == _cookies.end())
        return 1;
    return 0;
}

void ParssedRequest::assign_content_type()
{
    // std::map<std::string, std::string>::iterator it = _headers.find("Content-Type");
    std::string content_type_value = _headers["Content-Type"];
    std::istringstream stream(content_type_value);
    std::string cookie_pair;

    // std::cout << RED << content_type_value << WHIET << std::endl; // multipart/form-data; boundary=----geckoformboundarya62674fb9d5e16f0d6991b29e3e0e5d5
    _content_type_values.clear();
    while (std::getline(stream, cookie_pair, ';'))
    {
        // Trim whitespace from the entire cookie pair first
        cookie_pair = trim(cookie_pair);
        if (cookie_pair.empty())
            continue;

        size_t equals_pos = cookie_pair.find('=');

        // Case 1: "key=value" format
        if (equals_pos != std::string::npos)
        {
            std::string key = cookie_pair.substr(0, equals_pos);
            std::string value = cookie_pair.substr(equals_pos + 1);
            key = trim(key);
            value = trim(value);

            if (!key.empty())
            { // Only insert valid keys
                _content_type_values[key] = value;
            }
        }
        // Case 2: Malformed cookie (no '='), treat as empty value
        else if (!cookie_pair.empty())
        {
            std::string key = trim(cookie_pair);
            if (!key.empty())
            {
                _content_type_values[key] = ""; // Explicit empty value
            }
        }
    }
}

void ParssedRequest::assign_boundary()
{
    std::map<std::string, std::string>::iterator it = _content_type_values.find("boundary");
    if (it != _content_type_values.end())
    {
        _boundary = it->second;
        // std::cout << "Boundary: " << _boundary << std::endl;
    }
    else
    {
        // std::cerr << "Boundary not found in Content-Type header.\n";
        _boundary = "";
    }
}

void ParssedRequest::assign_Disposition_map(std::string key_values)
{
    std::istringstream stream(key_values);
    std::string cookie_pair;
    Content_Disposition_values.clear();

    while (std::getline(stream, cookie_pair, ';'))
    {
        // Trim whitespace from the entire cookie pair first
        cookie_pair = trim(cookie_pair);
        if (cookie_pair.empty())
            continue;

        size_t equals_pos = cookie_pair.find('=');

        // Case 1: "key=value" format
        if (equals_pos != std::string::npos)
        {
            std::string key = cookie_pair.substr(0, equals_pos);
            std::string value = cookie_pair.substr(equals_pos + 1);
            key = trim(key);
            value = trim(value);

            if (!key.empty())
            { // Only insert valid keys
                Content_Disposition_values[key] = value;
            }
        }
        // Case 2: Malformed cookie (no '='), treat as empty value
        else if (!cookie_pair.empty())
        {
            std::string key = trim(cookie_pair);
            if (!key.empty())
            {
                Content_Disposition_values[key] = ""; // Explicit empty value
            }
        }
    }
}

void ParssedRequest::parse_body()
{
    if (_boundary.empty())
    {
        // std::cerr << "Boundary is not set, cannot parse body.\n";
        return;
    }
    std::string full_boundary = "--" + _boundary;
    std::string end_boundary = full_boundary + "--";
    std::string result;

    size_t pos = 0;

    while (true)
    {
        // Find the start of the next boundary
        size_t start = _body.find(full_boundary, pos);
        if (start == std::string::npos)
            break;
        start += full_boundary.size();

        // Skip optional CRLF after boundary
        if (_body.substr(start, 2) == "\r\n")
            start += 2;

        // Find the next boundary
        size_t end = _body.find(full_boundary, start);
        if (end == std::string::npos)
        {
            end = _body.find(end_boundary, start);
            if (end == std::string::npos)
                break; // malformed
        }

        std::string part = _body.substr(start, end - start);

        // Remove trailing CRLF
        if (!part.empty() && part.back() == '\n')
        {
            part.pop_back();
            if (!part.empty() && part.back() == '\r')
                part.pop_back();
        }

        // --- Separate headers from body ---
        size_t header_end = part.find("\r\n\r\n");
        if (header_end != std::string::npos)
        {
            std::string headers_str = part.substr(0, header_end);
            std::string body_str = part.substr(header_end + 4); // Skip \r\n\r\n

            // Parse headers
            std::istringstream stream(headers_str);
            std::string line, key, value;
            while (std::getline(stream, line) && !line.empty() && line != "\r")
            {
                std::istringstream iss(line);
                if (std::getline(iss, key, ':') && std::getline(iss, value))
                {
                    if (trim(key) == "Content-Disposition")
                        assign_Disposition_map(trim(value));
                    else if (trim(key) == "Content-Type")
                        content_type_valbody = trim(value);
                }
            }

            // Append only the body part to result
            result += body_str;
        }
        pos = end;
    }
    parssed_body = result;
    // return result;
}

void ParssedRequest::check_body_size()
{
    correct_size = true;
    chunked = false;
    std::string::size_type pos = 0;

    if (_headers.find("Transfer-Encoding") != _headers.end() &&
        _headers["Transfer-Encoding"] == "chunked")
    {
        chunked = true;
        // int i = 0;

        while (pos < _body.size())
        {
            // Find the CRLF after the chunk size
            std::string::size_type end_line = _body.find("\r\n", pos);
            if (end_line == std::string::npos)
            {
                std::cout << RED << "end_line not found" << WHIET << std::endl;
                correct_size = false;
                return;
            }

            // Extract and parse chunk size (hex)
            std::string hex_size = _body.substr(pos, end_line - pos);
            size_t chunk_size;
            std::stringstream ss;
            ss << std::hex << hex_size;
            if (!(ss >> chunk_size))
            {
                std::cout << RED << "invalid chunk size" << WHIET << std::endl;
                correct_size = false;
                return;
            }

            pos = end_line + 2; // Move past CRLF

            // Zero-length chunk means end of body
            if (chunk_size == 0)
            {
                // Expect final CRLF after zero-size chunk
                if (_body.substr(pos, 2) != "\r\n")
                {
                    std::cout << RED << "missing final CRLF after last chunk" << WHIET << std::endl;
                    correct_size = false;
                }
                return; // Done parsing
            }

            // Check if chunk data fits in body
            if (pos + chunk_size + 2 > _body.size())
            {
                std::cout << RED << "not enough data for chunk" << WHIET << std::endl;
                correct_size = false;
                return;
            }

            // Extract chunk data
            std::string chunk_data = _body.substr(pos, chunk_size);
            pos += chunk_size;

            // Each chunk must end with CRLF
            if (_body.substr(pos, 2) != "\r\n")
            {
                std::cout << RED << "missing CRLF after chunk data" << WHIET << std::endl;
                correct_size = false;
                return;
            }
            pos += 2;

            // std::cout << "i :" << i++ << ", size: " << chunk_size << std::endl;
        }

        // If we exit loop without seeing 0-size chunk, it's invalid
        std::cout << RED << "no final zero-size chunk found" << WHIET << std::endl;
        correct_size = false;
    }

    std::cout << GREEN << pos << WHIET << std::endl;
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
            // else if (trim(key) == "Content-Type")
            //     assign_content_type(trim(value));
            _headers[trim(key)] = trim(value);
        }
        else
        {
            std::cerr << "Invalid header format: " << line << std::endl;
            return;
        }
    }
    cookies_response.clear();
    assignhost_port();
    is_cgi_path(_path);
    assign_params(_path);
    assign_pwithout_params();
    assign_content_type();
    assign_boundary();
    std::ostringstream body_stream;
    body_stream << stream.rdbuf();
    _body = body_stream.str();
    check_body_size();
    parse_body();
    nameValue = (Content_Disposition_values.count("name") > 0)
                    ? Content_Disposition_values["name"]
                    : "";

    filenameValue = (Content_Disposition_values.count("filename") > 0)
                        ? Content_Disposition_values["filename"]
                        : "";

    if (!filenameValue.empty() && filenameValue.front() == '"' && filenameValue.back() == '"')
    {
        filenameValue = filenameValue.substr(1, filenameValue.length() - 2);
    }
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
    // set cookies array
    int cookie_size = _cookies.size();
    int final_size = _params.size() + 3 + 1;
    if (cookie_size > 0)
        final_size += 1;                  // add HTTP_COOKIE if we have cookies
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
    std::map<std::string, std::string>::iterator cookies_begin = _cookies.begin();
    std::map<std::string, std::string>::iterator cookies_end = _cookies.end();

    // Only set HTTP_COOKIE if we have cookies
    if (cookies_begin != cookies_end)
    {
        std::string key_val = "HTTP_COOKIE=";
        bool first = true;

        while (cookies_begin != cookies_end)
        {
            if (!first)
            {
                key_val += "; ";
            }
            // Should URL-encode names/values here in production
            key_val += cookies_begin->first + "=" + cookies_begin->second;
            first = false;
            cookies_begin++;
        }

        envp[i] = new char[key_val.size() + 1];
        std::strcpy(envp[i], key_val.c_str());
        i++; // Don't forget to increment envp index!
    }
    std::string key = "REQUEST_METHOD";
    key_val = key + "=" + _method;
    envp[i] = new char[key_val.size() + 1];
    std::strcpy(envp[i], key_val.c_str());
    i++;
    key = "PATH_INFO";
    key_val = key + "=" + cgi_path;
    envp[i] = new char[key_val.size() + 1];
    std::strcpy(envp[i], key_val.c_str());
    i++;
    key = "SCRIPT_NAME";
    key_val = key + "=" + cgiPath;
    envp[i] = new char[key_val.size() + 1];
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
                return;
            }
            return;
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

void ParssedRequest::assign_full_cgi_path()
{
    std::string cgi_files = "cgi_files";
    const std::string prefix = "/cgi-bin/";
    if (_path_without_params.compare(0, prefix.size(), prefix) == 0)
    {
        std::string relative = _path_without_params.substr(prefix.size()); // remove "cgi-bin/"
        cgi_path = cgi_files + "/" + relative;
    }
    else
    {
        // Not a valid cgi-bin path
        cgi_path = "";
    }
    std::cout << cgi_path << std::endl;
    // std::cout << cgi_path <<
}

bool ParssedRequest::path_exists()
{
    // std::cout << MAGENTA << "checking if path exists: " << cgi_path << WHIET<<  std::endl;
    if (cgi_path == "cgi_files/")
        return (false);
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
    this->cookies_response += cookies_response;
}

std::string ParssedRequest::get_cookies_response(void)
{
    return this->cookies_response;
}
