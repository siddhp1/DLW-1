#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>

#include "cpu.hpp"
#include "memory.hpp"

class Emulator {
 private:
  CPU cpu;
  Memory memory;

 public:
  // ctors to initialize the CPU and memory
  // void load_program(const std::string& filename);
  // methods to run the fetch-decode-execute loop

  void print_memory();  // Temporary
};

#endif
