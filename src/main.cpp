/**
 * main.cpp
 * unix_shell
 * 
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#include <unistd.h>
#include "shell.hpp"

int main(int argc, const char *argv[]) {
  Shell shell(getpid());
  shell.run();
  
  return 0;
}
