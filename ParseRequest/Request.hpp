
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctime>
std::string trim(const std::string &str);
void printMap(std::map<std::string, std::string> myMap);

class ParssedRequest
{
private:
    std::string _request;
    std::string _host;
    std::string _port;
    std::string _method;
    std::string _path;
    std::string _path_without_params;
    std::string _path_info;
    std::string _script_name;
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::map<std::string, std::string> _cookies;
    std::map<std::string, std::string> _params;
    std::string _body;
    std::string cgi_output;
    std::string cookies_response;
    std::map<std::string, std::map<std::string, std::string> > _sessions;
    int status_code;
    bool cookie_avai;
    bool _cgi;
    std::string cgi_path;
public:
    // void assign_cookies();
    std::string generate_session_response();
    void assign_cookies(std::string key_value);
    std::map<std::string, std::string> get_cookies(){return _cookies;};
    void clear_params();
    int get_cookie_avai();
    void is_cgi_path(const std::string& path);
    void assign_cgi_output(std::string cgi_outpt);
    std::string get_cgi_output(void);
    bool is_cgi();
    void assign_cookies_response(std::string cookies_response);
    std::string get_cookies_response();
    std::string get_cgi_path();
    void assign_pwithout_params();
    void assign_full_cgi_path();
    bool path_exists();
    void assignhost_port();
    ParssedRequest(std::string request);
    ~ParssedRequest();
    void AssignRequestLine(std::string request);
    void AssignHeaders(std::string request);
    void AssignBody(std::string request);
    std::string getBody();
    std::string getMethod() const;
    std::string getPath() const;
    std::string getVersion() const;
    ParssedRequest();
    void AssignHeadersLine(std::string request);
    std::map<std::string, std::string> getHeaders();
    std::string get_host();
    std::string get_port();
    std::string GetReqeustLIne();
    void assign_params(std::string path);
    void printParams();
    std::map<std::string, std::string> getParams();
    void set_status_code(int stts_cd){this->status_code = stts_cd;};
    int get_status_code(){return (this->status_code);};
    char **get_env();
};
std::string execute_cgi(ParssedRequest &request, std::string file_path, char **env);
// std::string execute_cgi(std::string file_path, char **env);
void free_envp(char **envp);

#endif