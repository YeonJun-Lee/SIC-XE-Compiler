/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: main.cpp
 */

#include <iostream>
#include <exception>
#include "Assembler.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::cout << "Please provide source file(s) as argument" << std::endl;
    return 1;
  }

  Assembler assembler;

  try {
    assembler.assemble(argc, argv);
  }
  catch (const std::runtime_error &e) {
    std::cerr << "Runtime exception: " << e.what() << std::endl;
    return 1;
  }
  catch (const std::out_of_range &e) {
    std::cerr << "Out of range exception: " << e.what() << std::endl;
    return 1;
  }
  catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}