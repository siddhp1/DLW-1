#include "dlw1_emulator/cpu.hpp"

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "dlw1_emulator/instruction.hpp"
#include "dlw1_emulator/memory.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,hicpp-signed-bitwise,readability-magic-numbers)

uint8_t Cpu::ReadRegister(const RegisterId id) const noexcept {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
  return gpr[static_cast<std::size_t>(id)];
}

void Cpu::UpdateProcessorStatusWord(const uint8_t result) noexcept {
  psw = 0;
  if (result == 0) {
    psw |= 0b1U;
  } else if (static_cast<int8_t>(result) < 0) {
    psw |= 0b10U;
  }
}

void Cpu::WriteRegister(const RegisterId id, const uint8_t value) noexcept {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
  gpr[static_cast<std::size_t>(id)] = value;
}

Instruction Cpu::Decode() const noexcept {
  Instruction ins{};

  ins.raw = ir;

  // NOLINTNEXTLINE(readability-implicit-bool-conversion)
  const bool mode_bit = ir & 0b1U;

  const uint8_t opcode_bits = (ir >> 1U) & 0b111U;
  ins.opcode = static_cast<Opcode>(opcode_bits);

  switch (ins.opcode) {
    case Opcode::ADD:
    case Opcode::SUB:
      if (mode_bit) {
        ins.mode = AddressingMode::IMMEDIATE;
        ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
        ins.dest = static_cast<RegisterId>((ir >> 6U) & 0b11U);
        ins.imm = (ir >> 8U) & 0xFFU;
      } else {
        ins.mode = AddressingMode::REGISTER;
        ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
        ins.src2 = static_cast<RegisterId>((ir >> 6U) & 0b11U);
        ins.dest = static_cast<RegisterId>((ir >> 8U) & 0b11U);
      }
      break;
    case Opcode::LOAD:
      if (mode_bit) {
        if (((ir >> 4U) & 0b11U) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.dest = static_cast<RegisterId>((ir >> 6U) & 0b11U);
          ins.imm = (ir >> 8U) & 0xFFU;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
          ins.dest = static_cast<RegisterId>((ir >> 6U) & 0b11U);
          ins.imm = (ir >> 8U) & 0xFFU;
        }
      } else {
        if (((ir >> 6U) & 0b11U) == 0) {
          ins.mode = AddressingMode::REGISTER;
          ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
          ins.dest = static_cast<RegisterId>((ir >> 8U) & 0b11U);
        } else {
          ins.imm = (ir >> 8U) & 0xFFU;  // Bank switch
        }
      }
      break;
    case Opcode::STORE:
      if (mode_bit) {
        if (((ir >> 4U) & 0b11U) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.src = static_cast<RegisterId>((ir >> 6U) & 0b11U);
          ins.imm = (ir >> 8U) & 0xFFU;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
          ins.src2 = static_cast<RegisterId>((ir >> 6U) & 0b11U);
          ins.imm = (ir >> 8U) & 0xFFU;
        }
      } else {
        if (((ir >> 6U) & 0b11U) == 0) {
          ins.mode = AddressingMode::REGISTER;
          ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
          ins.dest = static_cast<RegisterId>((ir >> 8U) & 0b11U);
        } else {
          ins.mode = AddressingMode::NONE;
          ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
          ins.dest = static_cast<RegisterId>((ir >> 8U) & 0b11U);
        }
      }
      break;
    case Opcode::JUMP:
    case Opcode::JUMPZ:
    case Opcode::JUMPNZ:
    case Opcode::JUMPN:
      if (mode_bit) {
        if (((ir >> 4U) & 0b11U) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.imm = (ir >> 8U) & 0xFFU;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.imm =
              (ir >> 7U) & 0x1FFU;  // Using 9-bit immedaite for relative jumps
        }
      } else {
        if ((ir >> 6U) == 0) {
          ins.mode = AddressingMode::REGISTER;
          ins.src = static_cast<RegisterId>((ir >> 4U) & 0b11U);
        } else {
          break;  // Halt
        }
      }
      break;
  }
  return ins;
}

void Cpu::Execute(const Instruction& ins, Memory& memory) noexcept {
  switch (ins.opcode) {
    case Opcode::ADD:
    case Opcode::SUB: {
      const uint8_t x = ReadRegister(ins.src);

      const uint8_t y = (ins.mode == AddressingMode::IMMEDIATE)
                            ? ins.imm
                            : ReadRegister(ins.src2);

      const uint8_t result = (ins.opcode == Opcode::ADD) ? x + y : x - y;

      WriteRegister(ins.dest, result);
      UpdateProcessorStatusWord(result);
      break;
    }
    case Opcode::LOAD:
      switch (ins.mode) {
        case AddressingMode::IMMEDIATE:
          WriteRegister(ins.dest, memory.ReadByte(ins.imm));
          break;
        case AddressingMode::REGISTER:
          WriteRegister(ins.dest, memory.ReadByte(ReadRegister(ins.src)));
          break;
        case AddressingMode::RELATIVE: {
          const uint8_t addr =
              ReadRegister(ins.src) + CalculateOffset(ins.imm, ins.opcode);
          WriteRegister(ins.dest, memory.ReadByte(addr));
          break;
        }
        case AddressingMode::NONE:
          memory.SetCurrentBank(ins.imm);
        default:
          break;
      }
      break;
    case Opcode::STORE:
      switch (ins.mode) {
        case AddressingMode::IMMEDIATE:
          memory.WriteByte(ins.imm, ReadRegister(ins.src));
          break;
        case AddressingMode::REGISTER:
          memory.WriteByte(ReadRegister(ins.dest), ReadRegister(ins.src));
          break;
        case AddressingMode::RELATIVE: {
          const uint8_t addr =
              ReadRegister(ins.src) + CalculateOffset(ins.imm, ins.opcode);
          memory.WriteByte(addr, ReadRegister(ins.src2));
          break;
        }
        case AddressingMode::NONE:
          WriteRegister(ins.dest, ReadRegister(ins.src));
          break;
        default:
          break;
      }
      break;
    case Opcode::JUMP:
    case Opcode::JUMPZ:
    case Opcode::JUMPNZ:
    case Opcode::JUMPN: {
      uint8_t addr{};

      switch (ins.mode) {
        case AddressingMode::IMMEDIATE:
          addr = ins.imm;
          break;
        case AddressingMode::REGISTER:
          addr = ReadRegister(ins.src);
          break;
        case AddressingMode::RELATIVE: {
          addr = pc + CalculateOffset(ins.imm, ins.opcode);
          break;
        }
        case AddressingMode::NONE:
          halted = true;
          return;
        default:
          break;
      }

      bool execute_jump = false;
      switch (ins.opcode) {
        case Opcode::JUMP:
          execute_jump = true;
          break;
        case Opcode::JUMPZ:
          execute_jump = psw == 0b01;
          break;
        case Opcode::JUMPNZ:
          execute_jump = psw == 0b00;
          break;
        case Opcode::JUMPN:
          execute_jump = psw == 0b10;
          break;
        default:
          break;
      }

      if (execute_jump) {
        pc = addr;
      }

      break;
    }
    default:
      break;
  }
}

void Cpu::Fetch(const Memory& memory) noexcept {
  if (pc > 254) {
    halted = true;
    return;
  }

  const uint8_t high = memory.ReadByte(pc++);
  const uint8_t low = memory.ReadByte(pc++);
  ir = (high << 8U) | low;
}

bool Cpu::GetHalted() const noexcept { return halted; }

uint16_t Cpu::GetIr() const noexcept { return ir; }

uint8_t Cpu::GetPc() const noexcept { return pc; }

uint8_t Cpu::GetPsw() const noexcept { return psw; }

uint8_t Cpu::GetRegister(RegisterId id) const noexcept {
  return ReadRegister(id);
}

int16_t Cpu::CalculateOffset(uint16_t imm, Opcode opcode) noexcept {
  if (opcode == Opcode::LOAD || opcode == Opcode::STORE) {
    // Process as an 8-bit immediate
    return static_cast<int16_t>(static_cast<int8_t>(imm));
  } else {
    // Process as a 9-bit immediate
    imm &= 0x1FFU;
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    imm = (imm & 0x100U) ? (imm | ~0x1FFU) : imm;  // Sign extend to 16 bits
    return static_cast<int16_t>(imm);
  }
}

std::ostream& operator<<(std::ostream& os, const Cpu& cpu) {
  std::vector<std::string> lines;

  {
    std::ostringstream oss;
    oss << std::bitset<8>(cpu.GetRegister(RegisterId::A)) << " "
        << std::bitset<8>(cpu.GetRegister(RegisterId::B)) << " "
        << std::bitset<8>(cpu.GetRegister(RegisterId::C)) << " "
        << std::bitset<8>(cpu.GetRegister(RegisterId::D));
    lines.push_back(oss.str());
  }

  {
    std::ostringstream oss;
    oss << "PC: " << static_cast<int>(cpu.GetPc()) << "   PSW: ";
    if (cpu.GetPsw() == 0b01) {
      oss << "ZERO";
    } else if (cpu.GetPsw() == 0b10) {
      oss << "NEGATIVE";
    } else {
      oss << "EMPTY";
    }
    lines.push_back(oss.str());
  }

  {
    std::ostringstream oss;
    oss << "IR: " << std::bitset<16>(cpu.GetIr());
    lines.push_back(oss.str());
  }

  // Calculate the maximum line length
  size_t max_length = 0;
  for (const auto& line : lines) {
    max_length = std::max(line.size(), max_length);
  }

  // Top border
  os << "+" << std::string(max_length + 2, '-') << "+\n";

  // CPU state content
  for (const auto& line : lines) {
    os << "| " << line << std::string(max_length - line.size(), ' ') << " |\n";
  }

  // Bottom border
  os << "+" << std::string(max_length + 2, '-') << "+";

  return os;
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,hicpp-signed-bitwise,readability-magic-numbers)
