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
  size_t bytes_read = 0;

  uint8_t byte = 0;
  while (program_file.read(reinterpret_cast<char*>(&byte), 1)) {
    memory.WriteByte(address++, byte);

    if (address == 255) {
      address = 0;
      memory.SetCurrentBank(memory.GetCurrentBank() + 1);
    }

    bytes_read++;
  }

  std::cout << "Total bytes read: " << bytes_read << std::endl;
}

void Emulator::Run() {
  std::cout << memory;

  while (!cpu.halted) {
    cpu.Fetch(memory);

    Instruction instruction = cpu.Decode();
    std::cout << instruction << std::endl;

    cpu.Execute(instruction, memory);

    // Add logging for each cycle
  }

  std::cout << memory;
}
