#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <iostream>

enum class AddressingMode {
  REGISTER,
  IMMEDIATE,
  RELATIVE,
  NONE,
};

enum class Opcode : uint8_t {
  ADD = 0b000,
  SUB = 0b001,
  LOAD = 0b010,
  STORE = 0b011,
  JUMP = 0b100,
  JUMPZ = 0b101,
  JUMPNZ = 0b110,
  JUMPN = 0b111,
};

enum class RegisterId : uint8_t {
  A = 0b00,
  B = 0b01,
  C = 0b10,
  D = 0b11,
  NONE = 0b11 + 1,  // For creating instances of Instruction
};

struct Instruction {
  AddressingMode mode = AddressingMode::NONE;
  Opcode opcode;

  RegisterId src = RegisterId::NONE;
  RegisterId src2 = RegisterId::NONE;
  RegisterId dest = RegisterId::NONE;

  // All immediate values will be 8 bits except for relative jump addresses,
  // which will be 9 bits
  uint16_t imm;

  uint16_t encoded;  // Raw instruction
};

std::ostream& operator<<(std::ostream& os, const Instruction& ins);

#endif
