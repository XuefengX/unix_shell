/**
 * shell.hpp
 * unix_shell
 *
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#pragma once

#include <unistd.h>
#include <set>
#include <iterator>
#include <sys/types.h>
#include <signal.h>
#include "shelpers.hpp"

class Shell{
public:
    Shell(); 
    // init the shell
    int run();
    pid_t get_shell_pid();
private:
    pid_t shell_pid;
    std::set<pid_t> background_apps;
    // run shell builtins
    // return true if builtin is run
    bool builtins(std::vector<std::string> cla_tokens);
    void wait_for_foreground_app(std::vector<Command> cmds, std::vector<pid_t> pids);
    // Check if the background app is 
    // finished (zombie)
    void check_status_of_background_app();
};