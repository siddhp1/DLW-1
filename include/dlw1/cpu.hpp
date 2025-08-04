#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>

#include "instruction.hpp"
#include "memory.hpp"

class CPU {
 private:
  std::array<uint8_t, 4> gpr;  // 4 8-bit (1 byte) general purpose registers
  uint16_t ir;                 // 16-bit (2 byte) instruction register
  uint8_t pc;                  // 8-bit (1 byte) program counter
  uint8_t psw : 2;             // 2-bit processor status word

  uint8_t ReadRegister(const RegisterId register_id) const;
  void UpdateProcessorStatusWord(const uint8_t result);
  void WriteRegister(const RegisterId register_id, const uint8_t value);

 public:
  bool halted;

  CPU() : gpr{}, ir{0}, pc{0}, psw{0}, halted{false} {}
  // State constructor for unit testing
  CPU(std::array<uint8_t, 4> gpr, uint16_t ir, uint8_t pc, uint8_t psw,
      bool halted)
      : gpr{gpr}, ir{ir}, pc{pc}, psw{psw}, halted{halted} {}

  Instruction Decode() const;
  void Execute(const Instruction& instruction, Memory& memory);
  void Fetch(const Memory& memory);
  uint16_t GetIr() const;  // For unit tests
  uint8_t GetPc() const;
  uint8_t GetPsw() const;                             // For unit tests
  uint8_t GetRegister(RegisterId register_id) const;  // For unit tests

  static int16_t CalculateOffset(uint16_t imm, Opcode opcode);
};

#endif
