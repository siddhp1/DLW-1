#include "dlw1/emulator.hpp"

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

#include "dlw1/memory.hpp"

void Emulator::LoadProgram(const std::string& file_path) {
  std::ifstream program_file(file_path, std::ios::binary);

  if (!program_file) {
    // Add error handling here
  }

  uint8_t address = 0;
  size_t iteration = 0;  // Temporary

  uint8_t byte = 0;
  while (program_file.read(reinterpret_cast<char*>(&byte), 1)) {
    // Temporary
    std::cout << std::bitset<8>(byte);
    if (iteration > 0 && iteration % 2 != 0) {
      std::cout << std::endl;
    }

    memory.WriteByte(address, byte);

    if (address == 255) {
      address = 0;
      memory.SetCurrentBank(memory.GetCurrentBank() + 1);
    }

    iteration++;
  }
}

void Emulator::Run() {
  size_t iteration = 0;

  // Running indefinitely until halt functionality is added
  while (iteration < 100) {
    cpu.Fetch(memory);
    Instruction instruction = cpu.Decode();
    cpu.Execute(instruction, memory);

    // Add logging for each cycle
    std::cout << "Iteration: " << iteration++ << std::endl;  // Temporary
  }
}
