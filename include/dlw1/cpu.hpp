#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>

#include "instruction.hpp"
#include "memory.hpp"

class CPU {
 private:
  std::array<uint8_t, 4> gpr = {
      0};               // 4 8-bit (1 byte) general purpose registers
  uint16_t ir = 0;      // 16-bit (2 byte) instruction register
  uint8_t pc = 0;       // 8-bit (1 byte) program counter
  uint8_t psw : 2 = 0;  // 2-bit processor status word

  uint8_t ReadRegister(const RegisterId register_id);
  void UpdateProcessorStatusWord(const uint8_t result);
  void WriteRegister(const RegisterId register_id, const uint8_t value);

 public:
  Instruction Decode(uint16_t raw);
  void Execute(const Instruction& instruction, Memory& memory);
  void Fetch(const Memory& memory);
};

#endif
