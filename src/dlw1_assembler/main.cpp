#include <cstdlib>

#include "dlw1_assembler/assembler.hpp"

// clang-tidy reports false positive
// NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
  const Assembler assembler{};
  assembler.Assemble("sample_program.s");

  return EXIT_SUCCESS;
}
