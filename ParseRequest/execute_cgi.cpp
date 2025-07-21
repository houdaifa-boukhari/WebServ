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

std::string execute_cgi(std::string file_path, char **env)
{
    std::string c_path;
    std::string output;
    // (void)env;
    if (is_suffix(file_path, ".py"))
    {
        c_path = "/usr/local/bin/python3.7";
    }else if (is_suffix(file_path, ".php"))
    {
        c_path = "/usr/bin/php";
    }else
    {
        std::cout << "not a correct extension" << std::endl;
        return "";
    }
    
    char *const args[] = {
        const_cast<char *>(c_path.c_str()),
        const_cast<char *>(file_path.c_str()),
        NULL
    };


    std::string temp_filename = "/tmp/cgi_output_" +  std::to_string(time(NULL)) + "_";
    std::string temp_outfilename = "/tmp/cgi_input" +  std::to_string(time(NULL)) + "_";
    int pid = fork();
    if (pid == 0)
    {
        // child process
        // change with
        int output_fd = open(temp_filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600);
        if (output_fd == -1) {
            std::cerr << "Failed to open output file" << std::endl;
            std::exit(1);
        }
        if (dup2(output_fd, 1) == -1 || dup2(output_fd, 2) == -1)
        {
            std::cerr << "Failed to dup output file" << std::endl;
            std::exit(1);
        }
        close(output_fd);
        execve(c_path.c_str(), args, env);
        write(2, "execve failed\n", 15);
    }else
    {
        int output_fd = open(temp_filename.c_str(), O_RDWR | O_CREAT, 0600);
        int status;
        waitpid(pid, &status, 0);
        // if (WIFEXITED(status)) {
        //     int exit_code = WEXITSTATUS(status);
        //     if (exit_code == 1) {
        //         std::cerr << "Child exited with code 1 (open or dup2 failure)\n";
        //     } else if (exit_code == 127) {
        //         std::cerr << "execve failed in child\n";
        //     } else if (exit_code != 0) {
        //         std::cerr << "Child exited with error code: " << exit_code << std::endl;
        //     }
        // } else if (WIFSIGNALED(status)) {
        //     std::cerr << "Child terminated by signal: " << WTERMSIG(status) << std::endl;
        // } else {
        //     std::cerr << "Unknown child termination\n";
        // }
        char buffer[128];
        int bytesRead;
        while ((bytesRead = read(output_fd, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            output += buffer;
        }
        if (output == "execve failed\n")
            std::cout << "execve failedd" << std::endl;
        close(output_fd);
    }
    return output;
}
