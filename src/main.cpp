/**
 * main.cpp
 * unix_shell
 *
 * Created by Xuefeng Xu on 2/2/20.
 * Copyright © 2020 Xuefeng Xu. All rights reserved.
 */

#include "shell.hpp"

int main(int argc, const char *argv[]) {
  Shell shell;
  if(shell.run() != 0)
    std::cerr << "shell error" << std::endl;
  return 0;
}
