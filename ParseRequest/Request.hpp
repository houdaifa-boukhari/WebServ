
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

#define WHIET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
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
    std::map<std::string, std::string> _content_type_values;
    std::map<std::string, std::string> Content_Disposition_values;
    std::string _boundary;
    std::string _body;
    std::string cgi_output;
    std::string cookies_response;
    std::string content_type_valbody;
    std::string nameValue;
    std::string filenameValue;
    std::string parssed_body;
    std::string cgiPath;
    std::map<std::string, std::map<std::string, std::string> > _sessions;
    int status_code;
    // bool cookie_avai;
    bool _cgi;
    bool correct_size;
    bool chunked;
    std::string cgi_path;
public:
    // void assign_cookies();
    // std::string generate_session_response();
    void check_body_size();
    void assign_cgi_path(std::string cgi_path){this->cgiPath = cgi_path;};
    std::string getCgiPath() { return cgiPath; };
    bool get_correct_size() { return correct_size; };
    std::string getNameValue() { return nameValue; };
    std::string getFilenameValue() { return filenameValue; };
    void assign_Disposition_map(std::string key_values);
    void assign_cookies(std::string key_value);
    std::string getparssed_body() { return parssed_body; };
    // void assign_file_name() {this->file_name = file_name;};
    void parse_body();
    void assign_boundary();
    std::map<std::string, std::string> GetcontentdisPositionMap() {return Content_Disposition_values;};
    std::string get_boundary() {return _boundary;};
    std::map<std::string, std::string> get_cookies(){return _cookies;};
    std::string getcontent_type_valbody() {return content_type_valbody;};
    void clear_params();
    int get_cookie_avai();
    void is_cgi_path(const std::string& path);
    void assign_cgi_output(std::string cgi_outpt);
    std::map<std::string, std::string> get_content_type_values() {return _content_type_values;};
    std::string get_cgi_output(void);
    bool is_cgi();
    void assign_cookies_response(std::string cookies_response);
    void assign_content_type();
    std::string get_cookies_response();
    std::string get_cgi_path();
    void assign_pwithout_params();
    void assign_full_cgi_path();
    bool path_exists();
    void assignhost_port();
    ParssedRequest(std::string request);
    std::string get_path_without_params() {return _path_without_params;};
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