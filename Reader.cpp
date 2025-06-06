/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: Reader.cpp
 */

#include "Reader.h"
#include "AxeUtil.h"
#include <iostream>

Reader::Reader(std::string filename) {
  std::cout << "Reading file: " << filename << std::endl;
  fileToRead.exceptions(std::ifstream::failbit);
  fileToRead.open(filename);
}

Reader::~Reader() {
  if (fileToRead.is_open()) {
    fileToRead.close();
  }
  std::cout << "Reader destructor called" << std::endl;
}

std::vector<std::string> Reader::readSicLine() {
  std::vector<std::string> toReturn;
  std::string line;
  if (!fileToRead.eof() && std::getline(fileToRead, line)) {
    // return the entire comment line
    if (line[0] == '.') {
      return {line};
    }

    toReturn.push_back(line.substr(0, AxeUtil::symbolWidth));
    toReturn.push_back(line.substr(AxeUtil::symbolWidth, AxeUtil::instructionWidth));
    toReturn.push_back(line.substr(AxeUtil::symbolWidth + AxeUtil::instructionWidth, AxeUtil::operandWidth));
    if (line.size() > (AxeUtil::symbolWidth + AxeUtil::instructionWidth + AxeUtil::operandWidth)) {
      toReturn.push_back(line.substr(AxeUtil::symbolWidth + AxeUtil::instructionWidth + AxeUtil::operandWidth));
    }
  }
  else {
    return {};
  }

  return toReturn;
}

std::vector<std::string> Reader::readTempLine() {
  std::vector<std::string> toReturn;
  std::string line;
  if (!fileToRead.eof() && std::getline(fileToRead, line)) {
    // return the entire comment line
    if (line[0] == '.') {
      return {line};
    }

    toReturn.push_back(line.substr(0, AxeUtil::addressWidth));
    toReturn.push_back(line.substr(AxeUtil::addressWidth + 1, AxeUtil::symbolWidth));
    toReturn.push_back(line.substr(AxeUtil::addressWidth + AxeUtil::symbolWidth + 1, 1));
    toReturn.push_back(line.substr(AxeUtil::addressWidth + AxeUtil::symbolWidth + 2, AxeUtil::instructionWidth));
    toReturn.push_back(line.substr(AxeUtil::addressWidth + AxeUtil::symbolWidth + AxeUtil::instructionWidth + 3, 1));
    toReturn.push_back(line.substr(AxeUtil::addressWidth + AxeUtil::symbolWidth + AxeUtil::instructionWidth + 4, AxeUtil::operandWidth));
  }
  else {
    return {};
  }
  return toReturn;
}