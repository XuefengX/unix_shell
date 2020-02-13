/**
 * shell.cpp
 * unix_shell
 *
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#include "shell.hpp"

Shell::Shell(){
    this->shell_pid = getpid();
}

int Shell::run() {
  std::string cla;
  std::cout << "shell runs at PID: " << shell_pid << std::endl;
  std::cout << "$ ";
  while (getline(std::cin, cla)) {
    std::vector<std::string> cla_tokens = tokenize(cla);
    if (builtins(cla_tokens)) continue;
    std::vector<Command> cmds = getCommands(cla_tokens);
    int rc = getpid();
    pid_t pids[cmds.size()];
    for (int index = 0; index < cmds.size(); index++) {
      Command cmd = cmds[index];
      rc = fork();
      if (rc < 0) {
        std::cerr << "fork failed" << std::endl;
        exit(1);
      } else if (rc == 0) {
        pids[index] = getpid();
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
    if (rc > 0) {
      for (int index = 0; index < cmds.size(); index++) {
        int rc_wait = waitpid(rc, nullptr, 0);
        std::cout << "\nparent: " << getpid()
                  << " finish executing program PID: " << rc_wait << std::endl;
        std::cout << "$ ";
      }
    }
  }
  return 1;
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
    std::cout << "$ ";
  } else if (cla_tokens[0] == "pwd") {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << cwd << std::endl;
    std::cout << "$ ";
  } else if (cla_tokens[0].find("=") != std::string::npos) {
    size_t pos = cla_tokens[0].find("=");
    std::string environment_variable = cla_tokens[0].substr(0, pos);
    std::string value = cla_tokens[0].substr(pos + 1);
    if (setenv(environment_variable.c_str(), value.c_str(), 1) == -1)
      std::cerr << "cannot set environment variable" << std::endl;
    std::cout << "$ ";
  } else {
    has_builtin = false;
  }
  return has_builtin;
}

pid_t Shell::get_shell_pid(){
    return shell_pid;
}