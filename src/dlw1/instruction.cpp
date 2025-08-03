#include "dlw1/instruction.hpp"

#include <bitset>
#include <cstdint>
#include <iostream>

#include "dlw1/cpu.hpp"

std::ostream& operator<<(std::ostream& os, const AddressingMode& mode) {
  switch (mode) {
    case AddressingMode::REGISTER:
      return os << "REGISTER";
    case AddressingMode::IMMEDIATE:
      return os << "IMMEDIATE";
    case AddressingMode::RELATIVE:
      return os << "RELATIVE";
    default:
      return os << "NONE";
  }
}

std::ostream& operator<<(std::ostream& os, const Opcode& code) {
  switch (code) {
    case Opcode::ADD:
      return os << "ADD";
    case Opcode::SUB:
      return os << "SUB";
    case Opcode::LOAD:
      return os << "LOAD";
    case Opcode::STORE:
      return os << "STORE";
    case Opcode::JUMP:
      return os << "JUMP";
    case Opcode::JUMPZ:
      return os << "JUMPZ";
    case Opcode::JUMPNZ:
      return os << "JUMPNZ";
    case Opcode::JUMPN:
      return os << "JUMPN";
    default:
      return os << "UNKNOWN";
  }
}

std::ostream& operator<<(std::ostream& os, const RegisterId& id) {
  switch (id) {
    case RegisterId::A:
      return os << "A";
    case RegisterId::B:
      return os << "B";
    case RegisterId::C:
      return os << "C";
    case RegisterId::D:
      return os << "D";
    default:
      return os << "NONE";
  }
}

std::ostream& operator<<(std::ostream& os, const Instruction& ins) {
  os << "Encoded: 0x" << std::hex << ins.encoded << " 0b" << std::dec
     << std::bitset<16>(ins.encoded) << std::endl;
  os << "Mode: " << ins.mode << std::endl;
  os << "Opcode: " << ins.opcode << std::endl;

  switch (ins.mode) {
    case AddressingMode::REGISTER:
      os << "Src1: " << ins.src << std::endl;
      os << "Src2: " << ins.src2 << std::endl;
      os << "Dest: " << ins.dest << std::endl;
      break;
    case AddressingMode::IMMEDIATE:
      os << "Src: " << ins.src << std::endl;
      os << "Imm: " << static_cast<int>(ins.imm) << std::endl;
      os << "Dest: " << ins.dest << std::endl;
      break;
    case AddressingMode::RELATIVE:
      if (ins.opcode == Opcode::STORE) {
        os << "Src: " << ins.src2 << std::endl;
      }
      os << "Base: " << ins.src << std::endl;
      os << "Offset: " << CPU::CalculateOffset(ins.imm, ins.opcode) << std::endl;
      if (ins.opcode == Opcode::LOAD) {
        os << "Dest: " << ins.dest << std::endl;
      }
      break;
    default:
      os << "Invalid addressing mode" << std::endl;
  }
  return os;
}
