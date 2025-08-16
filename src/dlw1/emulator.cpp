#include "dlw1/emulator.hpp"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "dlw1/instruction.hpp"
#include "dlw1/memory.hpp"
#include "logger/logger.hpp"

void Emulator::LoadProgram() {
  LOG_DEBUG("Loading program from: {}", config.program_file_path);

  std::ifstream program_file(config.program_file_path, std::ios::binary);
  if (!program_file) {
    throw std::runtime_error("Failed to open program file: " +
                             config.program_file_path);
  }

  try {
    uint8_t addr = 0;
    std::size_t bytes_read = 0;
    uint8_t bank = 0;
    memory.SetCurrentBank(bank);

    uint8_t byte = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    while (program_file.read(reinterpret_cast<char*>(&byte), 1)) {
      memory.WriteByte(addr++, byte);
      bytes_read++;

      // When we reach the end of a bank, move to the next bank
      if (addr == 0) {  // Overflow to 0 after 255
        bank++;
        if (bank >= memory.GetNumBanks()) {
          throw std::runtime_error(
              "Program too large: exceeds available memory banks");
        }
        memory.SetCurrentBank(bank);
      }
    }

    // Reset to bank 0 after loading
    memory.SetCurrentBank(0);

    if (program_file.bad()) {
      throw std::runtime_error("Error occured while reading program file");
    }

    LOG_INFO("Successfully loaded {} bytes from program file", bytes_read);
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load program: " +
                             std::string(e.what()));
  }
}

void Emulator::Run() {
  LOG_DEBUG("Starting emulator execution");
  std::cout << memory;

  size_t cycle_count = 0;

  while (!cpu.GetHalted()) {
    cycle_count++;

    cpu.Fetch(memory);
    const Instruction ins = cpu.Decode();
    std::cout << ins;

    LOG_DEBUG("Cycle {}: Executing instruction", cycle_count);
    cpu.Execute(ins, memory);
  }

  std::cout << memory;
  LOG_INFO("Emulation completed after {} cycles", cycle_count);
}
