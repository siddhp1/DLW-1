#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>

#include "instruction.hpp"

class CPU {
 private:
  uint8_t pc = 0;       // 8-bit (1 byte) program counter
  uint16_t ir = 0;      // 16-bit (2 byte) instruction register
  uint8_t psw : 4 = 0;  // 4-bit processor status word

  std::array<uint8_t, 4> gpr = {
      0};  // 4 8-bit (1 byte) general purpose registers

 public:
  // fetch method
  Instruction Decode(uint16_t raw);
  // execute method
  // accessors and mutators where needed
};

#endif
