/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: Assembler.h
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "Reader.h"
#include "Writer.h"

class Assembler {
  public:
    /**
    Parses command line arguments and check for errors

    @param argc is number of command line arguments
    @param arguments is array of arguments to parse
    */
    void const assemble(int argc, const char *arguments[]);
  private:
    /**
    Checks if filenames ending in ".sic" are given

    @param filename is a file name string to check
    */
    void const sicExtensionChecker(const std::string &filename);

    /**
    Strips trailing white spaces from tokenized strings

    @param stringToStrip is a tokenized string to strip
    */
    std::string const stripWhiteSpace(const std::string &stringToStrip);

    /**
    Perform pass one of assembly. Populate SYMTAB and LITTAB with symbols and addresses

    @param reader object to perform reading from files
    @param writer object to perform writing to files
    @param symtab table of symbols
    @param littab table of literals
    */
    void passOne(Reader* reader, Writer* writer, std::unordered_map<std::string, int> &symtab, std::unordered_map<std::string, int> &littab);

    /**
    Perform pass two of assembly. Uses SYMTAB and LITTAB to assemble object code

    @param reader object to perform reading from files
    @param writer object to perform writing to files
    @param symtab table of symbols
    @param littab table of literals
    */
    void passTwo(Reader* reader, Writer* writer, const std::unordered_map<std::string, int> &symtab, const std::unordered_map<std::string, int> &littab);

    std::unordered_map<std::string, int> symtab;
    std::unordered_map<std::string, int> littab;
    std::unique_ptr<Reader> reader;
    std::unique_ptr<Writer> writer;
};

#endif // ASSEMBLER_H