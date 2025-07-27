#include <fstream>
#include <iostream>

#include "emulator.h"

int main(void) {
  // std::ofstream output_file("output.txt");
  // std::cout.rdbuf(output_file.rdbuf());

  Emulator emulator;

  emulator.load_program("program.bin");
  // emulator.print_memory();

  return 0;
}
