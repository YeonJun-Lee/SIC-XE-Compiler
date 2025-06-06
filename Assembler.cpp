/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: Assembler.cpp
 */

#include "Assembler.h"
#include "AxeUtil.h"
#include <vector>
#include <string>
#include <cmath>
#include <exception>
#include <utility>

void const Assembler::sicExtensionChecker(const std::string &filename) {
  if (filename.length() > 4) {
    std::string fileExtension = filename.substr(filename.length() - 4);
    if (fileExtension.compare(".sic") != 0) {
      throw std::runtime_error( "Incorrect file ending in \"" + fileExtension + "\". Compiles \".sic\" files only.");
    }
  }
  else {
    throw std::runtime_error("File name too short. Compiles \".sic\" files only.");
  }
}

std::string const Assembler::stripWhiteSpace(const std::string &stringToStrip) {
  return stringToStrip.substr(0, stringToStrip.find_first_of("\r\n "));
}


void const Assembler::assemble(int argc, const char *arguments[]) {
  // convert to vector of strings for ease of handling :^)
  std::vector<std::string> files(arguments + 1, arguments + argc);

  for (unsigned i = 0; i < files.size(); i++) {
    // check for correct extension
    sicExtensionChecker(files.at(i));

    std::string filename = files.at(i).substr(0, files.at(i).length() - 4);

    reader = std::unique_ptr<Reader>(new Reader(files.at(i)));
    writer = std::unique_ptr<Writer>(new Writer(filename + ".temp"));
    
    passOne(reader.get(), writer.get(), symtab, littab);

    reader = std::unique_ptr<Reader>(new Reader(filename + ".temp"));
    writer = std::unique_ptr<Writer>(new Writer(filename + ".listing"));

    passTwo(reader.get(), writer.get(), symtab, littab);
  }

  // write file with SYMTAB and LITTAB
}

void Assembler::passOne(Reader* reader, Writer* writer, std::unordered_map<std::string, int> &symtab, std::unordered_map<std::string, int> &littab) {
  std::string programName;
  int locctr, address;
  std::vector<std::string> line = reader->readSicLine();

  // write comment lines straight to temp file
  while (line[0][0] == '.') {
    writer->writeLine(-1, line[0]);
    line = reader->readSicLine();
  }

  if (line[1].find("START") != line[1].npos) {
    programName = stripWhiteSpace(line[0]);
    locctr = std::stoi(line[2], nullptr, 0x10);
    writer->writeLine(locctr, line[0], false, line[1].substr(1), ' ', line[2]);
    line = reader->readSicLine();
  }
  else {
    locctr = 0x0000;
  }

  while (!line.empty() && line[1].find("END") == line[1].npos) {

    bool extendedFormat = false;
    address = locctr;
    // if not a comment line
    if (line[0][0] != '.') {
      // symbol declaration
      if (line[0].find_first_of(' ') != 0) {
        if (symtab.find(stripWhiteSpace(line[0])) == symtab.end()) {
          symtab.emplace(stripWhiteSpace(line[0]), address);
        }
        else {
          throw std::runtime_error("Repeat declaration of symbol: " + line[0].substr(0, line[0].find_first_of(' ')));
        }
      }

      // extended format?
      if (line[1][0] == '+') {
        extendedFormat = true;
      }
      else if (line[1][0] == ' ') {
        // do nothing
      }
      else {
        throw std::runtime_error("Something wrong when processing instruction: " + line[1]);
      }
      line[1] = line[1].substr(1);
      // instruction
      if (AxeUtil::optab.find(stripWhiteSpace(line[1])) != AxeUtil::optab.end()) {
        (extendedFormat) ? locctr += 4 : locctr += 3;
      }
      else if (AxeUtil::format2.find(stripWhiteSpace(line[1])) != AxeUtil::format2.end()) {
        locctr += 2;
      }
      else if (AxeUtil::format1.find(stripWhiteSpace(line[1])) != AxeUtil::format1.end()) {
        locctr += 1;
      }
      else if (stripWhiteSpace(line[1]).compare("WORD") == 0) {
        locctr += 3;
      }
      else if (stripWhiteSpace(line[1]).compare("RESW") == 0) {
        locctr += 3 * std::stoi(line[2]);
      }
      else if (stripWhiteSpace(line[1]).compare("RESB") == 0) {
        locctr += std::stoi(line[2]);
      }
      else if (stripWhiteSpace(line[1]).compare("BYTE") == 0) {
        std::string directiveOperand = stripWhiteSpace(line[2].substr(1));
        // character constant
        if (directiveOperand[0] == 'C') {
          locctr += directiveOperand.size() - 3;
        }
        // hex constant
        else if (directiveOperand[0] == 'X') {
          locctr += (directiveOperand.size() - 3) / 2;
        }
        // decimal constant
        else if (directiveOperand[0] == 'D') {
          int decimal = std::stoi(directiveOperand.substr(directiveOperand.find_first_of('\'') + 1, directiveOperand.find_last_of('\'') - 2)) - 1;
          while (decimal != 0) {
              decimal >>= 8;
              locctr++;
          }
        }
        else {
          // do nothing
        }
      }
      else {
        // do nothing
      }
    }

    writer->writeLine(address, stripWhiteSpace(line[0]), extendedFormat, line[1], ' ', line[2]);
    line = reader->readSicLine();
  }

  writer->writeLine(address, line[0], false, line[1].substr(1), ' ', line[2]);
}

void Assembler::passTwo(Reader* reader, Writer* writer, const std::unordered_map<std::string, int> &symtab, const std::unordered_map<std::string, int> &littab) {
  std::vector<std::string> line = reader->readTempLine();

  int baseRegister = -1;
  int operand;
  int programCounter;
  int objectCode;
  bool extendedFormat;

  char firstByte;
  char xbpe;
  while (!line.empty()) {

    operand = 0;
    objectCode = -1;
    extendedFormat = false;

    // write comment lines straight to file
    if (line[0][0] == '.') {
      writer->writeLine(-1, line[0]);
      line = reader->readTempLine();
      continue;
    }
    // last instruction line
    else if (stripWhiteSpace(line[3]).compare("END") == 0) {
      break;
    }
    else {
      std::cout << "|" << line[0];
      std::cout << "|" << line[1];
      std::cout << "|" << line[2];
      std::cout << "|" << line[3];
      std::cout << "|" << line[4];
      std::cout << "|" << line[5] << "| ";

      // if format 2
      if (AxeUtil::format2.find(stripWhiteSpace(line[3])) != AxeUtil::format2.end()) {
        std::cout << "format 2";
        objectCode = AxeUtil::format2.at(stripWhiteSpace(line[3])) << 8;
        // if operand1 is symbol
        if (symtab.find(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", "))) != symtab.end()) {
          objectCode += symtab.at(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", "))) << 4;
        }
        // if operand is a register
        if (AxeUtil::registers.find(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", "))) != AxeUtil::registers.end()) {
          objectCode += AxeUtil::registers.at(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", "))) << 4;
        }
        // if operand2 is symbol
        if (symtab.find(stripWhiteSpace(line[5]).substr(line[5].find_first_of(", "))) != symtab.end()) {
          objectCode += symtab.at(stripWhiteSpace(line[5]).substr(line[5].find_first_of(", ")));
        }
        // if operand2 is register
        if (AxeUtil::registers.find(stripWhiteSpace(line[5]).substr(line[5].find_first_of(", "))) != AxeUtil::registers.end()) {
          objectCode += AxeUtil::registers.at(stripWhiteSpace(line[5]).substr(line[5].find_first_of(", ")));
        }
      }
      // if line has opcode
      else if (AxeUtil::optab.find(stripWhiteSpace(line[3])) != AxeUtil::optab.end()) {
        programCounter = std::stoi(line[0], nullptr, 16) + 3;
        firstByte = AxeUtil::optab.at(stripWhiteSpace(line[3]));
        xbpe = 0;
        // if operand is a symbol
        if (symtab.find(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", "))) != symtab.end()) {
          operand = symtab.at(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", ")));
          // pc relative
          if ((operand - programCounter >= -2048) && (operand - programCounter <= 2047)) {
            std::cout << "pc relative ";
            operand -= programCounter;
            xbpe += 1 << 1;
          }
          // base relative
          else if ((baseRegister != -1) && (operand - baseRegister >= 0) && (operand - baseRegister <= 4095)) {
            std::cout << "base relative ";
            operand -= baseRegister;
            xbpe += 1 << 2;
          }
          // extended format?
          else {
            // do nothing
          }
        }


        // extended format?
        if (line[2][0] == '+') {
          std::cout << "extended ";
          programCounter += 1;
          xbpe += 1;
          extendedFormat = true;
        }
        // direct, indirect or immediate?
        if (line[4][0] == '#') {
          std::cout << "immediate ";
          firstByte += 0x1;
        }
        else if (line[4][0] == '@') {
          std::cout << "indirect ";
          firstByte += 0x2;
        }
        else {
          std::cout << "direct ";
          firstByte += 0x3;
        }
        // index addressing
        if (stripWhiteSpace(line[5]).substr(line[5].find_first_of(", ")).compare(",X") == 0) {
          std::cout << "index ";
          xbpe += 1 << 3;
        }

        if (extendedFormat) {
          objectCode = firstByte << 24;
          objectCode += xbpe << 20;
        }
        else {
          objectCode = firstByte << 16;
          objectCode += xbpe << 12;
        }
        objectCode += static_cast<unsigned int>(operand & 0xFFF);
      }
      // process directives
      else {
        std::string directive = stripWhiteSpace(line[3]);
        // setting base
        if (directive.compare("BASE") == 0) {
          if (symtab.find(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", "))) != symtab.end()) {
            baseRegister = symtab.at(stripWhiteSpace(line[5]).substr(0, line[5].find_first_of(", ")));
          }
        }
        // unsetting base
        if (directive.compare("NOBASE") == 0) {
          baseRegister = -1;
        }
        // BYTE
        if (directive.compare("BYTE") == 0 || directive.compare("WORD") == 0) {
          std::string directiveOperand = line[5].substr(line[5].find_first_of('\'') + 1, line[5].find_last_of('\'') - 2);
          int i = directiveOperand.size() - 1;
          switch(line[5][0]) {
            case 'X':
              std::cout << "hex directive ";
              objectCode = std::stoi(directiveOperand, nullptr, 16);
              break;
            case 'C':
              std::cout << "char directive ";
              objectCode = 0;
              for (char c : directiveOperand) {
                objectCode += static_cast<unsigned int>(c << (i * 8));
                i--;
              }
              break;
            case 'D':
              std::cout << "decimal directive ";
              objectCode = std::stoi(directiveOperand);
              break;
            default:
              std::cout << "decimal directive ";
              objectCode = std::stoi(directiveOperand);
              break;
          }
        }
      }
      std::cout << "\n";
    }

    writer->writeLine(std::stoi(line[0], nullptr, 16), line[1], extendedFormat, line[3], line[4][0], line[5], objectCode);

    line = reader->readTempLine();
  }

  writer->writeLine(std::stoi(line[0], nullptr, 16), line[1], false, line[3], line[4][0], line[5]);
}

