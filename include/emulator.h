#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>

#include "cpu.h"
#include "memory.h"

class Emulator {
 private:
  CPU cpu;
  Memory memory;

 public:
  // ctors to initialize the CPU and memory
  void load_program(const std::string& filename);
  // method to run the fetch-decode-execute loop

  void print_memory();  // Temporary
};

#endif
