#pragma once
#include <iostream>
#include <string>

class Shell{
public:
    Shell(int pid);
    void run();
    int get_shell_pid();   

private:
    int pid;
    void execute_handler(std::string cla); 
};
