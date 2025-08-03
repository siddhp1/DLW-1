#include <iostream>

#include "dlw1/emulator.hpp"

int main(void) {
  Emulator emulator{};

  emulator.LoadProgram("sample_program.bin");
  emulator.Run();

  return 0;
}
