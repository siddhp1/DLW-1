#include "dlw1/emulator.hpp"

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

#include "dlw1/memory.hpp"

void Emulator::LoadProgram(const std::string& file_path) {
  std::ifstream program(file_path, std::ios::binary);
  if (!program) {
    // Add error handling here
  }

  uint8_t addr = 0;
  size_t bytes_read = 0;

  uint8_t byte = 0;
  while (program.read(reinterpret_cast<char*>(&byte), 1)) {
    memory.WriteByte(addr++, byte);

    if (addr == 255) {
      addr = 0;
      memory.SetCurrentBank(memory.GetCurrentBank() + 1);
    }

    bytes_read++;
  }

  std::cout << "Total bytes read: " << bytes_read << std::endl;
}

void Emulator::Run() {
  std::cout << memory;

  while (!cpu.GetHalted()) {
    cpu.Fetch(memory);

    Instruction ins = cpu.Decode();
    std::cout << ins << std::endl;

    cpu.Execute(ins, memory);

    // Add logging for each cycle
  }

  std::cout << memory;
}
