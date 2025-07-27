#include "emulator.h"

#include <fstream>
#include <iostream>
#include <string>

#include "memory.h"

void Emulator::load_program(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);

  if (!file) throw std::runtime_error("Failed to open file: " + filename);

  size_t addr = 0;
  u_int8_t byte = 0;

  while (file.read(reinterpret_cast<char*>(&byte), 1)) {
    if (addr > 256) throw std::runtime_error("Program too large for memory");

    std::cout << byte << std::endl;

    memory.write8(addr++, byte);
  }
}

void Emulator::print_memory() { memory.print(); }
