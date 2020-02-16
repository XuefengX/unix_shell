/**
 * shell.cpp
 * unix_shell
 *
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#include "shell.hpp"
#include <readline/readline.h>
#include <readline/history.h>

Shell::Shell(){
    this->shell_pid = getpid();
}

int Shell::run() {
  char * buffer;
  std::cout << "shell runs at PID: " << shell_pid << std::endl;
  // Auto completion
  while ((buffer = readline("$ "))) {
    std::string cla(buffer);
    std::vector<std::string> cla_tokens = tokenize(cla);
    if (builtins(cla_tokens)) continue;
    std::vector<Command> cmds = getCommands(cla_tokens);
    std::vector<pid_t> pids(cmds.size());
    for (int index = 0; index < cmds.size(); index++) {
      Command cmd = cmds[index];
      pids[index] = fork();
      // Child program started
      if (pids[index] < 0) {
        std::cerr << "fork failed" << std::endl;
        exit(1);
      } else if (pids[index] == 0) {
        std::cout << "child program PID: " << getpid() << "\n" << std::endl;
        // with no pipe
        dup2(cmd.fdStdin, 0);
        dup2(cmd.fdStdout, 1);
        int err =
            execvp(cmd.argv[0], const_cast<char *const *>(cmd.argv.data()));
        std::cerr << "ERROR CODE: " << err << std::endl;
        exit(0);
      }
    }
    if (pids[0] > 0) {
      // Check foreground apps
      for (int index = 0; index < cmds.size(); index++) {
        if(!cmds[index].background){
          int rc_wait = waitpid(pids[index], nullptr, 0);
          std::cout << "\nparent: " << getpid()
                  << " finish executing program PID: " << rc_wait << std::endl;
        }else{
          // Store background app's pid
          if(background_apps.find(pids[index]) == background_apps.end())
            background_apps.insert(pids[index]);
        }
      }
      // Check background apps
      for (std::set<pid_t>::iterator itr = background_apps.begin(); itr != background_apps.end(); ) {
        int status;
        pid_t zombie = waitpid(*itr, &status, WNOHANG);
        if (zombie == *itr) {
          std::cout << "program finished: " << *itr << std::endl;
          std::set<pid_t>::iterator to_be_delete = itr++;
          background_apps.erase(to_be_delete);
        } else if (zombie == 0){
          std::cout << "still running on background: " << *itr << std::endl;
          itr++;
        } else{
          std::cout << "wrong" << *itr << std::endl;
          itr++;
        } 
      }
    }
  }
  return 0;
}

bool Shell::builtins(std::vector<std::string> cla_tokens) {
  bool has_builtin = true;
  if (cla_tokens[0] == "exit")
    exit(0);
  else if (cla_tokens[0] == "cd") {
    const char *path =
        cla_tokens.size() > 1
            ? cla_tokens[1] == "~" ? getenv("HOME") : cla_tokens[1].c_str()
            : getenv("HOME");
    if (chdir(path) == -1) std::cerr << "change directory failed" << std::endl;
  } else if (cla_tokens[0] == "pwd") {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << cwd << std::endl;
  } else if (cla_tokens[0].find("=") != std::string::npos) {
    // Set environment variables
    size_t pos = cla_tokens[0].find("=");
    std::string environment_variable = cla_tokens[0].substr(0, pos);
    std::string value = cla_tokens[0].substr(pos + 1);
    if (setenv(environment_variable.c_str(), value.c_str(), 1) == -1)
      std::cerr << "cannot set environment variable" << std::endl;
  } else {
    has_builtin = false;
  }
  return has_builtin;
}

pid_t Shell::get_shell_pid(){
    return shell_pid;
}