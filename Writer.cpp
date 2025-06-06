/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: Writer.cpp
 */

#include "Writer.h"
#include "AxeUtil.h"
#include <iomanip>

Writer::Writer(std::string filename) {
  std::cout << "Writing file: " << filename << std::endl;
  fileToWrite.exceptions(std::ifstream::failbit);
  fileToWrite.open(filename);
}

Writer::~Writer() {
  if (fileToWrite.is_open()) {
    fileToWrite.close();
  }
  std::cout << "Writer destructor called\n";
}

void Writer::writeLine(int address, const std::string &symbol, bool extendedFormat, const std::string &instruction, char operandModifier, const std::string &operand, int objectCode, const std::string &comment) {
  if (fileToWrite.is_open()) {
    // if a comment
    if (address == -1) {
      fileToWrite << symbol << std::endl;
    }
    else {
      // address field
      if (address <= 0xFFFF){
          fileToWrite.put(intToHex(((address & 0xF000) >> 12)));
          fileToWrite.put(intToHex(((address & 0x0F00) >> 8)));
          fileToWrite.put(intToHex(((address & 0x00F0) >> 4)));
          fileToWrite.put(intToHex((address & 0x000F)));
      }
      else {
        throw std::runtime_error("Writing address that is out of range");
      }
      fileToWrite << ' ';

      // symbol field
      fileToWrite << std::setw(AxeUtil::symbolWidth) << std::left << symbol;
      fileToWrite << std::setw(1);

      // instruction modifier field
      extendedFormat ? (fileToWrite << '+') : (fileToWrite << ' ');

      // instruction field
      fileToWrite << std::setw(AxeUtil::instructionWidth) << std::left << instruction;
      
      // operand modifier field
      fileToWrite << operandModifier;

      // operand field
      fileToWrite.width(AxeUtil::operandWidth);
      fileToWrite << std::left << operand;
      fileToWrite << ' ';



      // object code field
      if (objectCode == -1) {
        // do nothing
      }
      else if (instruction.substr(0, instruction.find_first_of(' ')).compare("WORD") == 0) {
        if (objectCode <= 0xFFFFFF) {
          fileToWrite.put(intToHex(((objectCode & 0xF00000) >> 20)));
          fileToWrite.put(intToHex(((objectCode & 0x0F0000) >> 16)));
          fileToWrite.put(intToHex(((objectCode & 0xF000) >> 12)));
          fileToWrite.put(intToHex(((objectCode & 0x0F00) >> 8)));
          fileToWrite.put(intToHex(((objectCode & 0xF0) >> 4)));
          fileToWrite.put(intToHex((objectCode & 0x0F)));
        }
        else {
          throw std::runtime_error("Writing more than 3 bytes to WORD");
        }
      }
      else {
        if (objectCode <= 0xFFFFFFFF) {
          if (objectCode > 0xFFFFFF) {
            fileToWrite.put(intToHex(((objectCode & 0xF0000000) >> 28)));
            fileToWrite.put(intToHex(((objectCode & 0x0F000000) >> 24)));
          }
          if (objectCode > 0xFFFF) {
            fileToWrite.put(intToHex(((objectCode & 0xF00000) >> 20)));
            fileToWrite.put(intToHex(((objectCode & 0x0F0000) >> 16)));
          }
          if (objectCode > 0xFF) {
            fileToWrite.put(intToHex(((objectCode & 0xF000) >> 12)));
            fileToWrite.put(intToHex(((objectCode & 0x0F00) >> 8)));
          }
          fileToWrite.put(intToHex(((objectCode & 0xF0) >> 4)));
          fileToWrite.put(intToHex((objectCode & 0x0F)));
        }
        else {
          throw std::runtime_error("Writing more than 4 bytes of instruction");
        }
      }

      // comment field
      fileToWrite << comment;
      fileToWrite << std::endl;
    }
  }
  else {
    throw std::runtime_error("Writing to unopened file");
  }
}

char Writer::intToHex(int i) {
  if (i <= 9) {
    return '0' + i;
  }
  else {
    return 'A' - 10 + i;
  }
}