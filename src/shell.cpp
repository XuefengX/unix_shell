#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.hpp"
#include "shelpers.hpp"

Shell::Shell(int pid) { this->pid = pid; }

void Shell::run() {
  std::string cla;
  std::cout << "shell runs at PID: " << pid << std::endl;
  std::cout << "$ ";
  while (getline(std::cin, cla)) {
    if (cla == "exit") exit(0);
    int rc = fork();
    if (rc < 0) {
      std::cerr << "fork failed" << std::endl;
      exit(1);
    } else if (rc == 0) {
      execute_handler(cla);
    } else {
      int rc_wait = waitpid(rc, nullptr, 0);
      std::cout << "\nfinish executing program PID: " << rc_wait << std::endl;
      std::cout << "$ ";
    }
  }
}

void Shell::execute_handler(std::string cla) {
  std::cout << "child program PID: " << getpid() << "\n" << std::endl;
  std::vector<Command> cmds = getCommands(tokenize(cla));
  dup2(cmds[0].fdStdin, 0);
  dup2(cmds[0].fdStdout, 1);
	int err = execvp(cmds[0].argv[0], const_cast<char *const *>(cmds[0].argv.data()));
  std::cerr << "ERROR CODE: " << err << std::endl;
}

int Shell::get_shell_pid() { return pid; }