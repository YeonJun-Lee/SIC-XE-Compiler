/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: Writer.h
 */

#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <iostream>
#include <fstream>

class Writer {
  public:
    Writer(std::string filename);
    virtual ~Writer();

    void writeLine(
      int address = -1,
      const std::string &symbol = std::string(),
      bool extendedFormat = false,
      const std::string &instruction = std::string(),
      char operandModifier = ' ',
      const std::string &operand = std::string(),
      int objectCode = -1,
      const std::string &comment = std::string()
    );
  private:
    char intToHex(int i);
    std::ofstream fileToWrite;
};

#endif // WRITER_H