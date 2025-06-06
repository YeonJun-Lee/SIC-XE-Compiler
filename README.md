# SIC/XE Assembler

## Overview

This project is an assembler for the SIC/XE architecture. It reads `.sic` source files, performs two-pass assembly, and generates a formatted `.listing` file with object code and symbol resolution.

## Authors

- YeonJun Lee


## Features

- Implements a two-pass assembler:
  - **Pass One**: Constructs the symbol and literal tables (`SYMTAB`, `LITTAB`)
  - **Pass Two**: Generates object code using the tables
- Supports:
  - Format 1, 2, 3, and 4 instructions
  - Directives: `START`, `END`, `BYTE`, `WORD`, `RESB`, `RESW`, `BASE`, `NOBASE`
  - Addressing modes: immediate (`#`), indirect (`@`), indexed (`,X`)
- Outputs intermediate `.temp` and final `.listing` files

## File Structure

| File              | Description                                       |
|-------------------|---------------------------------------------------|
| `main.cpp`        | Entry point; handles input and runs the assembler |
| `Assembler.cpp/h` | Manages parsing, symbol/literal tables, and passes|
| `Reader.cpp/h`    | Reads and tokenizes `.sic` and `.temp` files      |
| `Writer.cpp/h`    | Outputs intermediate and listing files            |
| `AxeUtil.h`       | Utilities for parsing, formatting, and instruction info |
| `run.sh`          | Script to build and run the assembler             |
| `makefile`        | Build automation using `clang++`                  |
| `P2sample.sic`    | Sample input program                              |
| `P2sample.lis`    | Example output listing file                       |
| `P2sample.sym`    | Generated symbol table                            |

## How to Build and Run

### Prerequisites

- C++ compiler (`clang++`)
- Unix-like environment (tested with macOS/Linux)

### Instructions

```bash
# Clone the repository and enter the project directory
git clone https://github.com/yourusername/sicxe-assembler.git
cd sicxe-assembler

# Run the build and execution script
./run.sh
