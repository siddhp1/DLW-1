#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>

#include "cpu.hpp"
#include "memory.hpp"

class Emulator {
 private:
  Cpu cpu;
  Memory memory;

 public:
  void LoadProgram(const std::string& file_path);
  void Run();
};

#endif
