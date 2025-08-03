#include "dlw1/cpu.hpp"

#include <gtest/gtest.h>

#include "dlw1/instruction.hpp"
#include "dlw1/memory.hpp"

TEST(CPUDecodeTest, DecodeAddImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000010011000001;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::ADD);
  EXPECT_EQ(instruction.src, RegisterId::A);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::D);
  EXPECT_EQ(instruction.imm, 4);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeAddRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000001001000000;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::ADD);
  EXPECT_EQ(instruction.src, RegisterId::A);
  EXPECT_EQ(instruction.src2, RegisterId::B);
  EXPECT_EQ(instruction.dest, RegisterId::C);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeSubImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000010011000011;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::SUB);
  EXPECT_EQ(instruction.src, RegisterId::A);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::D);
  EXPECT_EQ(instruction.imm, 4);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeSubRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000001001000010;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::SUB);
  EXPECT_EQ(instruction.src, RegisterId::A);
  EXPECT_EQ(instruction.src2, RegisterId::B);
  EXPECT_EQ(instruction.dest, RegisterId::C);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeLoadImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0100001000000101;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::LOAD);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::A);
  EXPECT_EQ(instruction.imm, 66);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeLoadRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000000100000100;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::LOAD);
  EXPECT_EQ(instruction.src, RegisterId::A);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::B);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeLoadRelative) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0100001010110101;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::RELATIVE);
  EXPECT_EQ(instruction.opcode, Opcode::LOAD);
  EXPECT_EQ(instruction.src, RegisterId::D);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::C);
  EXPECT_EQ(instruction.imm, 66);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeBankSwitch) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000100011110100;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::NONE);
  EXPECT_EQ(instruction.opcode, Opcode::LOAD);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 8);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeStoreImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1100001000000111;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::STORE);
  EXPECT_EQ(instruction.src, RegisterId::A);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 194);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeStoreRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000001000010110;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::STORE);
  EXPECT_EQ(instruction.src, RegisterId::B);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::C);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeStoreRelative) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1100001000110111;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::RELATIVE);
  EXPECT_EQ(instruction.opcode, Opcode::STORE);
  EXPECT_EQ(instruction.src, RegisterId::D);
  EXPECT_EQ(instruction.src2, RegisterId::A);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 194);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1111111100001001;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMP);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000000000011000;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::JUMP);
  EXPECT_EQ(instruction.src, RegisterId::B);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpRelative) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0111111110011001;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::RELATIVE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMP);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeHalt) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1111111100001000;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::NONE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMP);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpzImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1111111100001011;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPZ);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpzRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000000000011010;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPZ);
  EXPECT_EQ(instruction.src, RegisterId::B);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpzRelative) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0111111110011011;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::RELATIVE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPZ);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpnzImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1111111100001101;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPNZ);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpnzRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000000000011100;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPNZ);
  EXPECT_EQ(instruction.src, RegisterId::B);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpnzRelative) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0111111110011101;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::RELATIVE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPNZ);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpnImmediate) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b1111111100001111;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::IMMEDIATE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPN);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpnRegister) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0000000000011110;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::REGISTER);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPN);
  EXPECT_EQ(instruction.src, RegisterId::B);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 0);
  EXPECT_EQ(instruction.encoded, raw);
}

TEST(CPUDecodeTest, DecodeJumpnRelative) {
  CPU cpu;
  Memory memory;

  uint16_t raw = 0b0111111110011111;

  Instruction instruction = cpu.Decode(raw);

  EXPECT_EQ(instruction.mode, AddressingMode::RELATIVE);
  EXPECT_EQ(instruction.opcode, Opcode::JUMPN);
  EXPECT_EQ(instruction.src, RegisterId::NONE);
  EXPECT_EQ(instruction.src2, RegisterId::NONE);
  EXPECT_EQ(instruction.dest, RegisterId::NONE);
  EXPECT_EQ(instruction.imm, 255);
  EXPECT_EQ(instruction.encoded, raw);
}

// For each execute test, create CPU and Memory, create an Instruction object,
// execute on that object, check registers, memory, PSW, and PC to see results
// TEST(CPUExecuteTest, ExecuteAddImmediate) { return; }

class LoadStoreOffsetTest : public ::testing::TestWithParam<Opcode> {};

TEST_P(LoadStoreOffsetTest, PositiveOffset) {
  uint16_t immediate = 0b01010101;
  int8_t offset = CPU::CalculateOffset(immediate, GetParam()); 
  EXPECT_EQ(offset, 85);
}

TEST_P(LoadStoreOffsetTest, NegativeOffset) {
  uint16_t immediate = 0b11010101;
  int8_t offset = CPU::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, -43);
}

TEST_P(LoadStoreOffsetTest, ZeroOffset) {
  uint16_t immediate = 0b00000000;
  int8_t offset = CPU::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, 0);
}

INSTANTIATE_TEST_SUITE_P(CPUCalculateLoadStoreOffsetTest, LoadStoreOffsetTest,
                         ::testing::Values(Opcode::LOAD, Opcode::STORE));

class JumpOffsetTest : public ::testing::TestWithParam<Opcode> {};

TEST_P(JumpOffsetTest, PositiveOffset) {
  uint16_t immediate = 0b010000010;
  int16_t offset = CPU::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, 130);
}

TEST_P(JumpOffsetTest, NegativeOffset) {
  uint16_t immediate = 0b100000110;
  int16_t offset = CPU::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, -250);
}

TEST_P(JumpOffsetTest, ZeroOffset) {
  uint16_t immediate = 0b000000000;
  int16_t offset = CPU::CalculateOffset(immediate, GetParam()); 
  EXPECT_EQ(offset, 0);
}

INSTANTIATE_TEST_SUITE_P(CPUCalculateJumpOffsetTest, JumpOffsetTest,
                         ::testing::Values(Opcode::JUMP, Opcode::JUMPZ,
                                           Opcode::JUMPNZ, Opcode::JUMPN));
