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
  void LoadProgram(const std::string& filename);
  void Run();
};

#endif
