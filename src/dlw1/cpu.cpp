#include "dlw1/cpu.hpp"

#include "dlw1/instruction.hpp"

Instruction CPU::Decode(uint16_t raw) {
  Instruction ins{};

  ins.encoded = raw;

  bool mode_flag = raw & 0b1;

  uint8_t opcode_bits = (raw >> 1) & 0b111;
  ins.opcode = static_cast<Opcode>(opcode_bits);

  switch (ins.opcode) {
    case Opcode::ADD:
    case Opcode::SUB:
      if (mode_flag) {
        ins.mode = AddressingMode::IMMEDIATE;
        ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);
        ins.dest = static_cast<RegisterId>((raw >> 6) & 0b11);
        ins.imm = (raw >> 8) & 0b11111111;
      } else {
        ins.mode = AddressingMode::REGISTER;
        ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);
        ins.src2 = static_cast<RegisterId>((raw >> 6) & 0b11);
        ins.dest = static_cast<RegisterId>((raw >> 8) & 0b11);
      }
      break;
    case Opcode::LOAD:
      if (mode_flag) {
        if (((raw >> 4) & 0b11) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.dest = static_cast<RegisterId>((raw >> 6) & 0b11);
          ins.imm = (raw >> 8) & 0b11111111;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);  // Base
          ins.dest = static_cast<RegisterId>((raw >> 6) & 0b11);
          ins.imm = (raw >> 8) & 0b11111111;
        }
      } else {
        ins.mode = AddressingMode::REGISTER;
        ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);
        ins.dest = static_cast<RegisterId>((raw >> 8) & 0b11);
      }
      break;
    case Opcode::STORE:
      if (mode_flag) {
        if (((raw >> 4) & 0b11) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.src = static_cast<RegisterId>((raw >> 6) & 0b11);
          ins.imm = (raw >> 8) & 0b11111111;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);  // Base
          ins.src2 = static_cast<RegisterId>((raw >> 6) & 0b11);
          ins.imm = (raw >> 8) & 0b11111111;
        }
      } else {
        ins.mode = AddressingMode::REGISTER;
        ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);
        ins.dest = static_cast<RegisterId>((raw >> 8) & 0b11);
      }
      break;
    case Opcode::JUMP:
    case Opcode::JUMPZ:
    case Opcode::JUMPNZ:
    case Opcode::JUMPN:
      if (mode_flag) {
        if (((raw >> 4) & 0b11) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.imm = (raw >> 8) & 0b11111111;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);  // Base
          ins.imm = (raw >> 8) & 0b11111111;
        }
      } else {
        ins.mode = AddressingMode::REGISTER;
        ins.src = static_cast<RegisterId>((raw >> 4) & 0b11);
      }
      break;
  }

  return ins;
}

void CPU::Fetch(const Memory& memory) {
  uint8_t high = memory.ReadByte(pc++);
  uint8_t low = memory.ReadByte(pc++);
  ir = (high << 8) | low;
}
