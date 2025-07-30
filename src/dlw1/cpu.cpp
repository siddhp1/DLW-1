#include "dlw1/cpu.hpp"

#include "dlw1/instruction.hpp"

uint8_t CPU::ReadRegister(const RegisterId register_id) {
  return gpr[static_cast<size_t>(register_id)];
}

void CPU::UpdateProcessorStatusWord(const uint8_t result) {
  psw = 0;
  if (result == 0) {
    psw |= 0b1;
  } else if (static_cast<int8_t>(result) < 0) {
    psw |= 0b10;
  }
}

void CPU::WriteRegister(const RegisterId register_id, const uint8_t value) {
  gpr[static_cast<size_t>(register_id)] = value;
}

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

void CPU::Execute(const Instruction& instruction, Memory& memory) {
  switch (instruction.opcode) {
    case Opcode::ADD:
    case Opcode::SUB: {
      uint8_t operand_a = ReadRegister(instruction.src);

      uint8_t operand_b = (instruction.mode == AddressingMode::IMMEDIATE)
                              ? instruction.imm
                              : ReadRegister(instruction.src2);

      uint8_t result = (instruction.opcode == Opcode::ADD)
                           ? operand_a + operand_b
                           : operand_a - operand_b;

      WriteRegister(instruction.dest, result);
      UpdateProcessorStatusWord(result);
      break;
    }
    case Opcode::LOAD:
      switch (instruction.mode) {
        case AddressingMode::IMMEDIATE:
          WriteRegister(instruction.dest, memory.ReadByte(instruction.imm));
          break;
        case AddressingMode::REGISTER:
          WriteRegister(instruction.dest,
                        memory.ReadByte(ReadRegister(instruction.src)));
          break;
        case AddressingMode::RELATIVE:
          int8_t offset = static_cast<int8_t>(instruction.imm);
          uint8_t address = ReadRegister(instruction.src) + offset;
          WriteRegister(instruction.dest, memory.ReadByte(address));
          break;
      }
      break;
    case Opcode::STORE:
      switch (instruction.mode) {
        case AddressingMode::IMMEDIATE:
          memory.WriteByte(instruction.imm, ReadRegister(instruction.src));
          break;
        case AddressingMode::REGISTER:
          memory.WriteByte(ReadRegister(instruction.dest),
                           ReadRegister(instruction.src));
          break;
        case AddressingMode::RELATIVE:
          int8_t offset = static_cast<int8_t>(instruction.imm);
          uint8_t address = ReadRegister(instruction.src) + offset;
          memory.WriteByte(address, ReadRegister(instruction.src2));
          break;
      }
      break;
    case Opcode::JUMP:
    case Opcode::JUMPZ:
    case Opcode::JUMPNZ:
    case Opcode::JUMPN: {
      uint8_t address;
      switch (instruction.mode) {
        case AddressingMode::IMMEDIATE:
          address = instruction.imm;
          break;
        case AddressingMode::REGISTER:
          address = ReadRegister(instruction.src);
          break;
        case AddressingMode::RELATIVE:
          int8_t offset = static_cast<int8_t>(instruction.imm);
          address = ReadRegister(instruction.src) + offset;
          break;
      }

      bool execute_jump = false;
      switch (instruction.opcode) {
        case Opcode::JUMP:
          execute_jump = true;
          break;
        case Opcode::JUMPZ:
          execute_jump = (psw & 0b1) != 0;
          break;
        case Opcode::JUMPNZ:
          execute_jump = (psw & 0b1) == 0;
          break;
        case Opcode::JUMPN:
          execute_jump = (psw & 0b10) != 0;
          break;
        default:  // Case is not possible
          break;
      }

      if (execute_jump) pc = address;
      break;
    }
  }
}

void CPU::Fetch(const Memory& memory) {
  uint8_t high = memory.ReadByte(pc++);
  uint8_t low = memory.ReadByte(pc++);
  ir = (high << 8) | low;
}
