/*
 * Authors: YeonJun Lee
 * Class: CS530
 * Project #2
 * File: AxeUtil.h
 */

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <ios>
#include <limits>

// constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

namespace AxeUtil {
  // Define column widths
  static const int addressWidth = 4;
  static const int symbolWidth = 9;
  static const int instructionWidth = 8;
  static const int operandWidth = 20;
  static const int opcodeWidth = 4;
  static const int commentWidth = 20;

  static const std::unordered_set<std::string> directive {
    "START", "END", "BYTE", "RESB", "WORD", "RESW", "BASE", "NOBASE", "*"
  };
  static const std::unordered_map<std::string, int> format2 = {
    {"ADDR"   , 0x90},
    {"CLEAR"  , 0xB4}, 
    {"COMPR"  , 0xA0},
    {"DIVR"   , 0x9C},
    {"MULR"   , 0x98},
    {"RMO"    , 0xAC},
    {"SHIFTL" , 0xA4},
    {"SHIFTR" , 0xA8},
    {"SUBR"   , 0x94},
    {"SVC"    , 0xB0},
    {"TIXR"   , 0xB8}
  };

  static const std::unordered_map<std::string, int> registers = {
    {"A", 0},
    {"X", 1},
    {"L", 2},
    {"B", 3},
    {"S", 4},
    {"T", 5},
    {"F", 6},
    {"PC", 8},
    {"SW", 9}
  };

  static const std::unordered_set<std::string> format1 {
    "FIX", "FLOAT", "HIO", "NORM", "SIO", "TIO"
  };

  static const std::unordered_map<std::string, int> optab = {
    {"ADD"    , 0x18},
    {"ADDF"   , 0x58},
    {"AND"    , 0x40},
    {"COMP"   , 0x28},
    {"COMPF"  , 0x88},
    {"DIV"    , 0x24},
    {"DIVF"   , 0x64},
    {"FIX"    , 0xC4},
    {"FLOAT"  , 0xC0},
    {"HIO"    , 0xF4},
    {"J"      , 0x3C},
    {"JEQ"    , 0x30},
    {"JGT"    , 0x34},
    {"JLT"    , 0x38},
    {"JSUB"   , 0x48},
    {"LDA"    , 0x00},
    {"LDB"    , 0x68},
    {"LDCH"   , 0x50},
    {"LDF"    , 0x70},
    {"LDL"    , 0x08},
    {"LDS"    , 0x6C},
    {"LDT"    , 0x74},
    {"LDX"    , 0x04},
    {"LPS"    , 0xD0},
    {"MUL"    , 0x20},
    {"MULF"   , 0x60},
    {"NORM"   , 0xC8},
    {"OR"     , 0x44},
    {"RD"     , 0xD8},
    {"RSUB"   , 0x4C},
    {"SIO"    , 0xF0},
    {"SSK"    , 0xEC},
    {"STA"    , 0x0C},
    {"STB"    , 0x78},
    {"STCH"   , 0x54},
    {"STF"    , 0x80},
    {"STI"    , 0xD4},
    {"STL"    , 0x14},
    {"STS"    , 0x7C},
    {"STSW"   , 0xE8},
    {"STT"    , 0x84},
    {"STX"    , 0x10},
    {"SUB"    , 0x1C},
    {"SUBF"   , 0x5C},
    {"TD"     , 0xE0},
    {"TIO"    , 0xF8},
    {"TIX"    , 0x2C},
    {"WD"     , 0xDC}
  };
}