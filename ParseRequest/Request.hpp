#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>

std::string trim(const std::string& str);
void printMap(std::map<std::string, std::string> myMap);

class ParssedRequest
{
    private:
        std::string _request;
        std::string _method;
        std::string _path;
        std::string _version;
        std::map<std::string, std::string> _headers;
        std::map<std::string, std::string> _params;
        std::string _body;
    public:
        ParssedRequest(std::string request);
        ~ParssedRequest();
        void AssignRequestLine(std::string request);
        void AssignHeaders(std::string request);
        void AssignBody(std::string request);
        std::string getBody();
        ParssedRequest();
        void AssignHeadersLine(std::string request);
        std::map<std::string, std::string> getHeaders();
        std::string GetReqeustLIne();
        void assign_params(std::string path);
        void printParams();
        std::map<std::string, std::string> getParams();
        char **get_env();
        // void AssignBody(std::string request);
        // assign request line
        // assign headers
        // assign body
};

std::string execute_cgi(std::string file_path, char **env);
void free_envp(char **envp);