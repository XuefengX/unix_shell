//
//  main.cpp
//  unix_shell
//
//  Created by Xuefeng Xu on 2/2/20.
//  Copyright © 2020 Xuefeng Xu. All rights reserved.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "shelpers.hpp"

int main(int argc, const char *argv[]) {
  std::cout << "shell runs at PID: " << getpid() << std::endl;
  std::cout << "$ ";
  std::string cla;
  while (getline(std::cin, cla)) {
    int rc = fork();
    if (rc < 0) {
      std::cerr << "fork failed" << std::endl;
      exit(1);
    } else if (rc == 0) {
      std::cout << "child program PID: " << getpid() << "\n" << std::endl;
      std::vector<Command> cmds = getCommands(tokenize(cla));
      int err = execvp(cmds[0].argv[0],
                       const_cast<char *const *>(cmds[0].argv.data()));
      std::cerr << "ERROR CODE: " << err << std::endl;
    } else {
      int rc_wait = waitpid(rc, nullptr, 0);
      std::cout << "\nfinish executing program PID: " << rc_wait << std::endl;
      std::cout << "$ ";
    }
  }
  return 0;
}
