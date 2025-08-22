#include "Request.hpp"

bool is_suffix(std::string str, std::string suffix)
{
    if (str.length() < suffix.length())
        return false;
    else if (str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0)
        return true;
    return false;
}

void printdoublearry(char **arr)
{
    for (int i = 0; arr[i]; i++)
    {
        std::cout << arr[i] << std::endl;
    }
}

void free_envp(char **envp)
{
    for (int i = 0; envp[i] != NULL; ++i)
        delete[] envp[i];
    delete[] envp;
}

char **return_envparams(std::map<std::string, std::string> params)
{
    std::map<std::string, std::string>::iterator begin = params.begin();
    std::map<std::string, std::string>::iterator end = params.end();
    std::string key_val;
    char **envp = new char *[params.size() + 1];
    envp[params.size()] = NULL;
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

unsigned int simple_random(unsigned int seed) {
    return (1103515245 * seed + 12345) % 100000;
}

std::string execute_cgi(ParssedRequest &request, std::string file_path, char **env)
{
    std::string c_path;
    std::string output;
    c_path = request.getCgiPath();
    char *const args[] = {
        const_cast<char *>(c_path.c_str()),
        const_cast<char *>(file_path.c_str()),
        NULL
    };

    std::clock_t clk = std::clock();
    unsigned int seed = static_cast<unsigned int>(clk);

    unsigned int unique_id = simple_random(seed);

    std::ostringstream oss;
    oss << clk << "_" << unique_id;

    std::string timestamp = oss.str();
    std::string temp_filename = "/tmp/cgi_output_" + timestamp;
    std::string temp_inputfilename = "/tmp/cgi_input_" + timestamp;
    int input_fd = open(temp_inputfilename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600);
    int writing_in = write(input_fd, request.getBody().c_str(), request.getBody().length());
    close(input_fd);
    if (writing_in == -1)
    {
        request.set_status_code(500);
        return "1";
    }
    int pid = fork();
    if (pid == 0)
    {
        int output_fd = open(temp_filename.c_str(), O_RDWR);
        int new_input_fd = open(temp_inputfilename.c_str(), O_RDONLY);
        if (output_fd == -1 || new_input_fd == -1)
            std::exit(12);
        if (dup2(output_fd, 1) == -1 || dup2(output_fd, 2) == -1 || dup2(new_input_fd, 0) == -1)
        {
            close(output_fd);
            std::exit(13);
        }
        close(output_fd);
        close(new_input_fd);
        execve(c_path.c_str(), args, env);
        std::exit(127);
    }else if (pid > 0)
    {
        int output_fd = open(temp_filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600);
        if (output_fd == -1)
        {
            request.set_status_code(500);
            return "2";
        }
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cerr << "CGI exited with code: " << WEXITSTATUS(status) << std::endl;
            request.set_status_code(500);
            close(output_fd);
            return "3";
        }
        char buffer[128];
        int bytesRead;
        while ((bytesRead = read(output_fd, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            output += buffer;
        }
        close(output_fd);
    }
    else
        request.set_status_code(500);
    if (request.get_status_code() != 500)
        request.set_status_code(200);
    return output;
}
