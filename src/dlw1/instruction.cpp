#include "dlw1/instruction.hpp"

#include <bitset>
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

std::ostream& operator<<(std::ostream& os, const Opcode& opcode) {
  switch (opcode) {
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
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,
  // readability-magic-numbers)
  os << "Raw: 0x" << std::hex << ins.raw << " 0b" << std::dec
     << std::bitset<16>(ins.raw) << "\n";
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  os << "Addressing Mode: " << ins.mode << "\n";
  os << "Opcode: " << ins.opcode << "\n";

  switch (ins.mode) {
    case AddressingMode::REGISTER:
      os << "Src1: " << ins.src << "\n";
      os << "Src2: " << ins.src2 << "\n";
      os << "Dest: " << ins.dest << "\n";
      break;
    case AddressingMode::IMMEDIATE:
      os << "Src: " << ins.src << "\n";
      os << "Imm: " << static_cast<int>(ins.imm) << "\n";
      os << "Dest: " << ins.dest << "\n";
      break;
    case AddressingMode::RELATIVE:
      if (ins.opcode == Opcode::STORE) {
        os << "Src: " << ins.src2 << "\n";
      }
      os << "Base: " << ins.src << "\n";
      os << "Offset: " << Cpu::CalculateOffset(ins.imm, ins.opcode) << "\n";
      if (ins.opcode == Opcode::LOAD) {
        os << "Dest: " << ins.dest << "\n";
      }
      break;
    default:
      os << "Invalid addressing mode" << "\n";
  }
  return os;
}
