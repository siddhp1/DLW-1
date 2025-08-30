#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>

#include "config.hpp"
#include "cpu.hpp"
#include "memory.hpp"

class Emulator {
 private:
  Cpu cpu;
  Memory memory;
  Config config;

 public:
  explicit Emulator(const Config& config)
      : config{config}, memory{config.num_banks} {}

  void LoadProgram();
  void Run();
};

#endif
