#include "dlw1/emulator.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include "dlw1/instruction.hpp"
#include "dlw1/memory.hpp"

void Emulator::LoadProgram() {
  std::ifstream program_file(config.program_file_path, std::ios::binary);
  if (!program_file) {
    // Add error handling here
  }

  uint8_t addr = 0;
  size_t bytes_read = 0;
  uint8_t bank = 0;
  memory.SetCurrentBank(bank);

  uint8_t byte = 0;
  while (program_file.read(reinterpret_cast<char*>(&byte), 1)) {
    memory.WriteByte(addr++, byte);
    bytes_read++;

    // When we reach the end of a bank, move to the next bank
    if (addr == 0) {  // Overflow to 0 after 255
      bank++;
      if (bank >= memory.GetNumBanks()) {
        // Add error handling here
      }
      memory.SetCurrentBank(bank);
    }

    // Reset to bank 0 after loading
    memory.SetCurrentBank(0);
  }

  // Make this a part of the logging after
  std::cout << "Total bytes read: " << bytes_read << "\n";
}

void Emulator::Run() {
  std::cout << memory;

  size_t cycle_count = 0;
  while (!cpu.GetHalted()) {
    cycle_count++;

    cpu.Fetch(memory);
    const Instruction ins = cpu.Decode();
    std::cout << ins << "\n";

    cpu.Execute(ins, memory);

    // Add logging for each cycle
  }

  std::cout << memory;
}
