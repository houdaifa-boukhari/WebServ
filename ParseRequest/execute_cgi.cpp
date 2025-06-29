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
    (void)env;
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
    // (void)output_fd;
    // printdoublearry(env2.data());
    int pid = fork();
    if (pid == 0)
    {
        // child process
        // change with
        int output_fd = open(temp_filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600);
        int input_fd = open(temp_outfilename.c_str(), O_RDONLY);
        dup2(input_fd, 0); // Duplicate read end to stdin
        dup2(output_fd, 1); // Duplicate write end to stdout
        dup2(output_fd, 2); // Duplicate write end to stdout
        // change with
        // dup2(input_fd, 0); // Duplicate read end to stdin
        close(input_fd); // Close unused read end
        close(output_fd); // Close write end after writing
        execve(c_path.c_str(), args, env);
        // perror("execve");
        write(2, "execve failed\n", 15);
    }else
    {
        // parent process
        // std::cout << "************parent process************" << std::endl;
        int output_fd = open(temp_filename.c_str(), O_RDWR | O_CREAT, 0600);
        waitpid(pid, NULL, 0);
        char buffer[128];
        int bytesRead;
        // lseek(output_fd, 0, SEEK_SET);
        while ((bytesRead = read(output_fd, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            output += buffer;
        }
        if (output == "execve failed\n")
        {
            std::cout << "execve failedd" << std::endl;
        }
        // std::cout << "************ output from parent ************ : " << output << std::endl;
        close(output_fd);
    }
    return output;
}
