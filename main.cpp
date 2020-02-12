/**
 * main.cpp
 * unix_shell
 *
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#include <unistd.h>
#include "shelpers.hpp"

int main(int argc, const char *argv[]) {
  std::string cla;
  std::cout << "shell runs at PID: " << getpid() << std::endl;
  std::cout << "$ ";
  while (getline(std::cin, cla)) {
    std::vector<std::string> cls_tokens = tokenize(cla);
    if (cla == "exit") exit(0);
    else if(cls_tokens[0] == "cd"){
        const char *path = cls_tokens.size() > 1 ? cls_tokens[1] == "~" ? getenv("HOME") : cls_tokens[1].c_str() : getenv("HOME");
        if(chdir(path) == -1) 
          std::cerr << "change directory failed" << std::endl;
        std::cout << "$ ";
        continue;
    }else if(cla == "pwd"){
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
      std::cout << cwd << std::endl;
      std::cout << "$ ";
      continue;
    }
    std::vector<Command> cmds = getCommands(cls_tokens);
    int rc = getpid();
    int pids[cmds.size()];
    for (int index = 0; index < cmds.size(); index++) {
      Command cmd = cmds[index];
      rc = fork();
      if (rc < 0) {
        std::cerr << "fork failed" << std::endl;
        exit(1);
      } else if (rc == 0) {
        pids[index] = getpid();
        std::cout << "child program PID: " << getpid() << "\n" << std::endl;
        for (int i = 0; i < cmds.size(); i++) {
          // with no pipe
          dup2(cmd.fdStdin, 0);
          dup2(cmd.fdStdout, 1);
          int err = execvp(cmd.argv[0],
                           const_cast<char *const *>(cmd.argv.data()));
          std::cerr << "ERROR CODE: " << err << std::endl;
          exit(0);
        }
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
  return 0;
}
