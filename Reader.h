/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: Reader.h
 */

#ifndef READER_H
#define READER_H

#include <string>
#include <fstream>
#include <vector>

class Reader {
  public:
    Reader(std::string filename);
    virtual ~Reader();

    std::vector<std::string> readSicLine();

    std::vector<std::string> readTempLine();
  private:
    std::ifstream fileToRead;
};

#endif // READER_H