#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <sys/types.h>

enum class Opcode : u_int8_t {
  ADD = 0b00,
  SUB = 0b001,
  LOAD = 0b010,
  STORE = 0b011,
  JUMP = 0b100,
  JUMPZ = 0b101,
  JUMPNZ = 0b110,
  JUMPN = 0b111,
};

enum class InstructionMode : u_int8_t {
  REGISTER = 0b0,
  IMMEDIATE = 0b1,
};

typedef u_int16_t binary_instruction;

struct Instruction {
  InstructionMode mode;
  Opcode opcode;

  u_int8_t src1 : 2;
  u_int8_t src2 : 2;
  u_int8_t dest : 2;
  u_int8_t imm;  // 8-bit immediate/offset

  // Internal flags
  bool use_imm;
  bool is_relative;
};

#endif
