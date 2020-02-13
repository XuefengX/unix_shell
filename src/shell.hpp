/**
 * shell.hpp
 * unix_shell
 *
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#pragma once

#include <unistd.h>
#include "shelpers.hpp"

class Shell{
public:
    Shell(); 
    int run();
    pid_t get_shell_pid();
private:
    pid_t shell_pid;
    // run shell builtins
    // return true if builtin is run
    bool builtins(std::vector<std::string> cla_tokens);

};