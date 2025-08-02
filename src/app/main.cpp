#include <iostream>

#include "dlw1/cpu.hpp"
#include "dlw1/emulator.hpp"
#include "dlw1/instruction.hpp"

// Repurpose this function to test writing a program to memory
void TestDecode() {
  CPU cpu;
  std::string input;
  uint16_t raw;

  while (true) {
    std::cout << "Enter a 16-bit instruction (hex prefix with 0x, binary with "
                 "0b, or 'q' to quit)"
              << std::endl;
    std::getline(std::cin, input);

    if (input == "q" || input == "quit") {
      return;
    }

    if (input.substr(0, 2) == "0x") {
      raw = static_cast<uint16_t>(std::stoul(input, nullptr, 16));
    } else if (input.substr(0, 2) == "0b") {
      raw = static_cast<uint16_t>(std::stoul(input.substr(2), nullptr, 2));
    } else {
      raw = static_cast<uint16_t>(std::stoul(input));
    }

    Instruction ins = cpu.Decode(raw);
    std::cout << ins;
  }
}

int main(void) {
  // TestDecode();

  Emulator emulator;

  emulator.LoadProgram("program.bin");
  emulator.Run();

  return 0;
}
