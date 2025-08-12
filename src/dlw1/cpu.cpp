#include "dlw1/cpu.hpp"

#include "dlw1/instruction.hpp"
#include "dlw1/memory.hpp"

uint8_t Cpu::ReadRegister(const RegisterId id) const {
  return gpr[static_cast<size_t>(id)];
}

void Cpu::UpdateProcessorStatusWord(const uint8_t result) {
  psw = 0;
  if (result == 0) {
    psw |= 0b1;
  } else if (static_cast<int8_t>(result) < 0) {
    psw |= 0b10;
  }
}

void Cpu::WriteRegister(const RegisterId id, const uint8_t value) {
  gpr[static_cast<size_t>(id)] = value;
}

Instruction Cpu::Decode() const {
  Instruction ins{};

  ins.raw = ir;

  bool mode_bit = ir & 0b1;

  uint8_t opcode_bits = (ir >> 1) & 0b111;
  ins.opcode = static_cast<Opcode>(opcode_bits);

  switch (ins.opcode) {
    case Opcode::ADD:
    case Opcode::SUB:
      if (mode_bit) {
        ins.mode = AddressingMode::IMMEDIATE;
        ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
        ins.dest = static_cast<RegisterId>((ir >> 6) & 0b11);
        ins.imm = (ir >> 8) & 0b11111111;
      } else {
        ins.mode = AddressingMode::REGISTER;
        ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
        ins.src2 = static_cast<RegisterId>((ir >> 6) & 0b11);
        ins.dest = static_cast<RegisterId>((ir >> 8) & 0b11);
      }
      break;
    case Opcode::LOAD:
      if (mode_bit) {
        if (((ir >> 4) & 0b11) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.dest = static_cast<RegisterId>((ir >> 6) & 0b11);
          ins.imm = (ir >> 8) & 0b11111111;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
          ins.dest = static_cast<RegisterId>((ir >> 6) & 0b11);
          ins.imm = (ir >> 8) & 0b11111111;
        }
      } else {
        if (((ir >> 6) & 0b11) == 0) {
          ins.mode = AddressingMode::REGISTER;
          ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
          ins.dest = static_cast<RegisterId>((ir >> 8) & 0b11);
        } else {
          ins.imm = (ir >> 8) & 0xFF;  // Bank switch
        }
      }
      break;
    case Opcode::STORE:
      if (mode_bit) {
        if (((ir >> 4) & 0b11) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.src = static_cast<RegisterId>((ir >> 6) & 0b11);
          ins.imm = (ir >> 8) & 0b11111111;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
          ins.src2 = static_cast<RegisterId>((ir >> 6) & 0b11);
          ins.imm = (ir >> 8) & 0b11111111;
        }
      } else {
        if (((ir >> 6) & 0b11) == 0) {
          ins.mode = AddressingMode::REGISTER;
          ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
          ins.dest = static_cast<RegisterId>((ir >> 8) & 0b11);
        } else {
          ins.mode = AddressingMode::NONE;
          ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
          ins.dest = static_cast<RegisterId>((ir >> 8) & 0b11);
        }
      }
      break;
    case Opcode::JUMP:
    case Opcode::JUMPZ:
    case Opcode::JUMPNZ:
    case Opcode::JUMPN:
      if (mode_bit) {
        if (((ir >> 4) & 0b11) == 0) {
          ins.mode = AddressingMode::IMMEDIATE;
          ins.imm = (ir >> 8) & 0xFF;
        } else {
          ins.mode = AddressingMode::RELATIVE;
          ins.imm =
              (ir >> 7) & 0x1FF;  // Using 9-bit immedaite for relative jumps
        }
      } else {
        if ((ir >> 6) == 0) {
          ins.mode = AddressingMode::REGISTER;
          ins.src = static_cast<RegisterId>((ir >> 4) & 0b11);
        } else {
          break;  // Halt
        }
      }
      break;
  }
  return ins;
}

void Cpu::Execute(const Instruction& ins, Memory& memory) {
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
      uint8_t addr;

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

void Cpu::Fetch(const Memory& memory) {
  if (pc > 254) {
    halted = true;
    return;
  }

  uint8_t high = memory.ReadByte(pc++);
  uint8_t low = memory.ReadByte(pc++);
  ir = (high << 8) | low;
}

bool Cpu::GetHalted() const { return halted; }

uint16_t Cpu::GetIr() const { return ir; }

uint8_t Cpu::GetPc() const { return pc; }

uint8_t Cpu::GetPsw() const { return psw; }

uint8_t Cpu::GetRegister(RegisterId id) const { return ReadRegister(id); }

int16_t Cpu::CalculateOffset(uint16_t imm, Opcode opcode) {
  if (opcode == Opcode::LOAD || opcode == Opcode::STORE) {
    // Process as an 8-bit immediate
    return static_cast<int16_t>(static_cast<int8_t>(imm));
  } else {
    // Process as a 9-bit immediate
    imm &= 0x1FF;
    imm = (imm & 0x100) ? (imm | ~0x1FF) : imm;  // Sign extend to 16 bits
    return static_cast<int16_t>(imm);
  }
}
