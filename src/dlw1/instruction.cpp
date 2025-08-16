#include "dlw1/instruction.hpp"

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  std::vector<std::string> lines;

  {
    std::ostringstream oss;
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    oss << "Raw: 0x" << std::hex << std::uppercase << std::setw(4)
        << std::setfill('0') << ins.raw << " 0b" << std::dec
        << std::bitset<16>(ins.raw);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    lines.push_back(oss.str());
  }

  {
    std::ostringstream oss;
    oss << "Addressing Mode: " << ins.mode;
    lines.push_back(oss.str());
  }

  {
    std::ostringstream oss;
    oss << "Opcode: " << ins.opcode;
    lines.push_back(oss.str());
  }

  switch (ins.mode) {
    case AddressingMode::REGISTER: {
      std::ostringstream oss;
      oss << "Src1: " << ins.src;
      lines.push_back(oss.str());
      std::ostringstream oss2;
      oss2 << "Src2: " << ins.src2;
      lines.push_back(oss2.str());
      std::ostringstream oss3;
      oss3 << "Dest: " << ins.dest;
      lines.push_back(oss3.str());
      break;
    }
    case AddressingMode::IMMEDIATE: {
      std::ostringstream oss;
      oss << "Src: " << ins.src;
      lines.push_back(oss.str());
      std::ostringstream oss2;
      oss2 << "Imm: " << static_cast<int>(ins.imm);
      lines.push_back(oss2.str());
      std::ostringstream oss3;
      oss3 << "Dest: " << ins.dest;
      lines.push_back(oss3.str());
      break;
    }
    case AddressingMode::RELATIVE: {
      if (ins.opcode == Opcode::STORE) {
        std::ostringstream oss;
        oss << "Src: " << ins.src2;
        lines.push_back(oss.str());
      }
      {
        std::ostringstream oss;
        oss << "Base: " << ins.src;
        lines.push_back(oss.str());
      }
      if (ins.opcode == Opcode::LOAD) {
        std::ostringstream oss;
        oss << "Offset: " << Cpu::CalculateOffset(ins.imm, ins.opcode);
        lines.push_back(oss.str());
        std::ostringstream oss2;
        oss2 << "Dest: " << ins.dest;
        lines.push_back(oss2.str());
      } else {
        std::ostringstream oss;
        oss << "Offset: " << Cpu::CalculateOffset(ins.imm, ins.opcode);
        lines.push_back(oss.str());
      }
      break;
    }
    default:
      lines.emplace_back("Other addressing mode");
  }

  // Calculate the maximum line length for proper padding
  size_t max_length = 0;
  for (const auto& line : lines) {
    max_length = std::max(line.size(), max_length);
  }

  // Top border
  os << "+" << std::string(max_length + 2, '-') << "+\n";

  // CPU state content with left/right padding
  for (const auto& line : lines) {
    os << "| " << line << std::string(max_length - line.size(), ' ') << " |\n";
  }

  // Bottom border
  os << "+" << std::string(max_length + 2, '-') << "+";

  return os;
}
