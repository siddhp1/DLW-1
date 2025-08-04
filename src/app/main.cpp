#include <iostream>

#include "dlw1/emulator.hpp"

int main(void) {
  Emulator em;

  em.LoadProgram("sample_program.bin");
  em.Run();

  return 0;
}
