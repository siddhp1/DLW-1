#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "cpu.h"
#include "memory.h"

class Simulator {
 private:
  CPU cpu;
  Memory memory;

 public:
  // ctors to initialize the CPU and memory
  // method to load program into memory
  // method to run the fetch-decode-execute loop
};

#endif
