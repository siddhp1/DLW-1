#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>

#include "instruction.hpp"
#include "memory.hpp"

class Cpu {
 private:
  std::array<uint8_t, 4> gpr;  // 4 8-bit (1 byte) general purpose registers
  bool halted;
  uint16_t ir;      // 16-bit (2 byte) instruction register
  uint8_t pc;       // 8-bit (1 byte) program counter
  uint8_t psw : 2;  // 2-bit processor status word

  [[nodiscard]] uint8_t ReadRegister(const RegisterId id) const noexcept;
  void UpdateProcessorStatusWord(const uint8_t result) noexcept;
  void WriteRegister(const RegisterId id, const uint8_t value) noexcept;

 public:
  Cpu() : gpr{}, ir{0}, pc{0}, psw{0}, halted{false} {}
  // State constructor for unit testing
  Cpu(std::array<uint8_t, 4> gpr, uint16_t ir, uint8_t pc, uint8_t psw,
      bool halted)
      : gpr{gpr}, ir{ir}, pc{pc}, psw{psw}, halted{halted} {}

  [[nodiscard]] Instruction Decode() const noexcept;
  void Execute(const Instruction& ins, Memory& memory) noexcept;
  void Fetch(const Memory& memory) noexcept;
  bool GetHalted() const noexcept;
  [[nodiscard]] uint16_t GetIr() const noexcept;
  [[nodiscard]] uint8_t GetPc() const noexcept;
  [[nodiscard]] uint8_t GetPsw() const noexcept;
  [[nodiscard]] uint8_t GetRegister(RegisterId id) const noexcept;

  [[nodiscard]] static int16_t CalculateOffset(uint16_t imm,
                                               Opcode opcode) noexcept;
};

#endif
