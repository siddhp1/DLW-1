#include "dlw1/cpu.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>
#include <vector>

#include "dlw1/instruction.hpp"
#include "dlw1/memory.hpp"
#include "gtest/gtest.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

class CpuDecodeTest
    : public ::testing::TestWithParam<
          std::tuple<uint16_t,        // Raw instruction
                     AddressingMode,  // Expected addressing mode
                     Opcode,          // Expected opcode
                     RegisterId,      // Expected src
                     RegisterId,      // Expected src2
                     RegisterId,      // Expected dest
                     uint16_t>> {};   // Expected imm

TEST_P(CpuDecodeTest, DecodeInstruction) {
  const auto& [raw, expected_mode, expected_opcode, expected_src, expected_src2,
               expected_dest, expected_imm] = GetParam();

  const Cpu cpu{{0, 0, 0, 0}, raw, 0, 0, false};

  const Instruction instruction = cpu.Decode();

  EXPECT_EQ(instruction.mode, expected_mode);
  EXPECT_EQ(instruction.opcode, expected_opcode);
  EXPECT_EQ(instruction.src, expected_src);
  EXPECT_EQ(instruction.src2, expected_src2);
  EXPECT_EQ(instruction.dest, expected_dest);
  EXPECT_EQ(instruction.imm, expected_imm);
  EXPECT_EQ(instruction.raw, raw);
}

INSTANTIATE_TEST_SUITE_P(
    ADD, CpuDecodeTest,
    ::testing::Values(std::make_tuple(0b0000010011000001,
                                      AddressingMode::IMMEDIATE, Opcode::ADD,
                                      RegisterId::A, RegisterId::NONE,
                                      RegisterId::D, 4),
                      std::make_tuple(0b0000001001000000,
                                      AddressingMode::REGISTER, Opcode::ADD,
                                      RegisterId::A, RegisterId::B,
                                      RegisterId::C, 0)));

INSTANTIATE_TEST_SUITE_P(
    SUB, CpuDecodeTest,
    ::testing::Values(std::make_tuple(0b0000010011000011,
                                      AddressingMode::IMMEDIATE, Opcode::SUB,
                                      RegisterId::A, RegisterId::NONE,
                                      RegisterId::D, 4),
                      std::make_tuple(0b0000001001000010,
                                      AddressingMode::REGISTER, Opcode::SUB,
                                      RegisterId::A, RegisterId::B,
                                      RegisterId::C, 0)));

INSTANTIATE_TEST_SUITE_P(
    LOAD, CpuDecodeTest,
    ::testing::Values(
        std::make_tuple(0b0100001000000101, AddressingMode::IMMEDIATE,
                        Opcode::LOAD, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::A, 66),
        std::make_tuple(0b0000000100000100, AddressingMode::REGISTER,
                        Opcode::LOAD, RegisterId::A, RegisterId::NONE,
                        RegisterId::B, 0),
        std::make_tuple(0b0100001010110101, AddressingMode::RELATIVE,
                        Opcode::LOAD, RegisterId::D, RegisterId::NONE,
                        RegisterId::C, 66),
        std::make_tuple(0b0000100011110100, AddressingMode::NONE, Opcode::LOAD,
                        RegisterId::NONE, RegisterId::NONE, RegisterId::NONE,
                        8)));

INSTANTIATE_TEST_SUITE_P(
    STORE, CpuDecodeTest,
    ::testing::Values(
        std::make_tuple(0b1100001000000111, AddressingMode::IMMEDIATE,
                        Opcode::STORE, RegisterId::A, RegisterId::NONE,
                        RegisterId::NONE, 194),
        std::make_tuple(0b0000001000010110, AddressingMode::REGISTER,
                        Opcode::STORE, RegisterId::B, RegisterId::NONE,
                        RegisterId::C, 0),
        std::make_tuple(0b1100001000110111, AddressingMode::RELATIVE,
                        Opcode::STORE, RegisterId::D, RegisterId::A,
                        RegisterId::NONE, 194),
        std::make_tuple(0b0000001011010110, AddressingMode::NONE, Opcode::STORE,
                        RegisterId::B, RegisterId::NONE, RegisterId::C, 0)));

INSTANTIATE_TEST_SUITE_P(
    JUMP, CpuDecodeTest,
    ::testing::Values(
        std::make_tuple(0b1111111100001001, AddressingMode::IMMEDIATE,
                        Opcode::JUMP, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255),
        std::make_tuple(0b0000000000011000, AddressingMode::REGISTER,
                        Opcode::JUMP, RegisterId::B, RegisterId::NONE,
                        RegisterId::NONE, 0),
        std::make_tuple(0b0111111110011001, AddressingMode::RELATIVE,
                        Opcode::JUMP, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255),
        std::make_tuple(0b1111111100001000, AddressingMode::NONE, Opcode::JUMP,
                        RegisterId::NONE, RegisterId::NONE, RegisterId::NONE,
                        0)));

INSTANTIATE_TEST_SUITE_P(
    JUMPZ, CpuDecodeTest,
    ::testing::Values(
        std::make_tuple(0b1111111100001011, AddressingMode::IMMEDIATE,
                        Opcode::JUMPZ, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255),
        std::make_tuple(0b0000000000011010, AddressingMode::REGISTER,
                        Opcode::JUMPZ, RegisterId::B, RegisterId::NONE,
                        RegisterId::NONE, 0),
        std::make_tuple(0b0111111110011011, AddressingMode::RELATIVE,
                        Opcode::JUMPZ, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255)));

INSTANTIATE_TEST_SUITE_P(
    JUMPNZ, CpuDecodeTest,
    ::testing::Values(
        std::make_tuple(0b1111111100001101, AddressingMode::IMMEDIATE,
                        Opcode::JUMPNZ, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255),
        std::make_tuple(0b0000000000011100, AddressingMode::REGISTER,
                        Opcode::JUMPNZ, RegisterId::B, RegisterId::NONE,
                        RegisterId::NONE, 0),
        std::make_tuple(0b0111111110011101, AddressingMode::RELATIVE,
                        Opcode::JUMPNZ, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255)));

INSTANTIATE_TEST_SUITE_P(
    JUMPN, CpuDecodeTest,
    ::testing::Values(
        std::make_tuple(0b1111111100001111, AddressingMode::IMMEDIATE,
                        Opcode::JUMPN, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255),
        std::make_tuple(0b0000000000011110, AddressingMode::REGISTER,
                        Opcode::JUMPN, RegisterId::B, RegisterId::NONE,
                        RegisterId::NONE, 0),
        std::make_tuple(0b0111111110011111, AddressingMode::RELATIVE,
                        Opcode::JUMPN, RegisterId::NONE, RegisterId::NONE,
                        RegisterId::NONE, 255)));

class CpuExecuteTest
    : public ::testing::TestWithParam<std::tuple<
          std::array<uint8_t, 4>,                         // Initial registers
          uint8_t,                                        // Initial pc
          uint8_t,                                        // Initial psw
          std::vector<std::pair<uint8_t, uint8_t>>,       // Initial memory
          Instruction,                                    // Instruction
          std::array<uint8_t, 4>,                         // Expected registers
          uint8_t,                                        // Expected pc
          uint8_t,                                        // Expected psw
          bool,                                           // Expected halted
          std::vector<std::pair<uint8_t, uint8_t>>>> {};  // Expected memory

TEST_P(CpuExecuteTest, ExecuteInstruction) {
  const auto& [initial_registers, initial_pc, initial_psw, initial_memory,
               instruction, expected_registers, expected_pc, expected_psw,
               expected_halted, expected_memory] = GetParam();

  Cpu cpu{initial_registers, 0, initial_pc, initial_psw, false};
  Memory memory;

  for (const auto& [addr, val] : initial_memory) {
    memory.WriteByte(addr, val);
  }

  cpu.Execute(instruction, memory);

  for (std::size_t i = 0; i < 4; ++i) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    EXPECT_EQ(cpu.GetRegister(static_cast<RegisterId>(i)),
              expected_registers[i]);
  }

  for (const auto& [addr, val] : expected_memory) {
    EXPECT_EQ(memory.ReadByte(addr), val);
  }

  EXPECT_EQ(cpu.GetPc(), expected_pc);
  EXPECT_EQ(cpu.GetPsw(), expected_psw);
  EXPECT_EQ(cpu.GetHalted(), expected_halted);
}

INSTANTIATE_TEST_SUITE_P(
    ADD, CpuExecuteTest,
    ::testing::Values(
        // ADD Immediate: A(5) + 10 = D(15)
        std::make_tuple(std::array<uint8_t, 4>{5, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::ADD,
                                    RegisterId::A, RegisterId::NONE,
                                    RegisterId::D, 10, 0},
                        std::array<uint8_t, 4>{5, 0, 0, 15}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // ADD Register: A(5) + B(10) = C(15)
        std::make_tuple(std::array<uint8_t, 4>{5, 10, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::REGISTER, Opcode::ADD,
                                    RegisterId::A, RegisterId::B, RegisterId::C,
                                    0, 0},
                        std::array<uint8_t, 4>{5, 10, 15, 0}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{})));

INSTANTIATE_TEST_SUITE_P(
    SUB, CpuExecuteTest,
    ::testing::Values(
        // SUB Immediate: A(20) - 20 = D(0)
        std::make_tuple(std::array<uint8_t, 4>{20, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::SUB,
                                    RegisterId::A, RegisterId::NONE,
                                    RegisterId::D, 20, 0},
                        std::array<uint8_t, 4>{20, 0, 0, 0}, 0, 0b01, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // SUB Register: A(5) - B(20) = C(-15)
        std::make_tuple(std::array<uint8_t, 4>{5, 20, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::REGISTER, Opcode::SUB,
                                    RegisterId::A, RegisterId::B, RegisterId::C,
                                    0, 0},
                        std::array<uint8_t, 4>{5, 20, 0b11110001, 0}, 0, 0b10,
                        false, std::vector<std::pair<uint8_t, uint8_t>>{})));

INSTANTIATE_TEST_SUITE_P(
    LOAD, CpuExecuteTest,
    ::testing::Values(
        // LOAD Immediate: Load value at addr 15 into A
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{{15, 42}},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::LOAD,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::A, 15, 0},
                        std::array<uint8_t, 4>{42, 0, 0, 0}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{{15, 42}}),
        // LOAD Register: Load B with value stored at addr in A
        std::make_tuple(std::array<uint8_t, 4>{15, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{{15, 42}},
                        Instruction{AddressingMode::REGISTER, Opcode::LOAD,
                                    RegisterId::A, RegisterId::NONE,
                                    RegisterId::B, 0, 0},
                        std::array<uint8_t, 4>{15, 42, 0, 0}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{{15, 42}}),
        // LOAD Relative: Load C with value from memory at addr(D+10)
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 5}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{{15, 42}},
                        Instruction{AddressingMode::RELATIVE, Opcode::LOAD,
                                    RegisterId::D, RegisterId::NONE,
                                    RegisterId::C, 10, 0},
                        std::array<uint8_t, 4>{0, 0, 42, 5}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{{15, 42}})));

INSTANTIATE_TEST_SUITE_P(
    STORE, CpuExecuteTest,
    ::testing::Values(
        // STORE Immediate: Store A(42) at address 100
        std::make_tuple(std::array<uint8_t, 4>{42, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::STORE,
                                    RegisterId::A, RegisterId::NONE,
                                    RegisterId::NONE, 100, 0},
                        std::array<uint8_t, 4>{42, 0, 0, 0}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{{100, 42}}),
        // STORE Register: Store B(42) at address in C(100)
        std::make_tuple(std::array<uint8_t, 4>{0, 42, 100, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{{100, 0}},
                        Instruction{AddressingMode::REGISTER, Opcode::STORE,
                                    RegisterId::B, RegisterId::NONE,
                                    RegisterId::C, 0, 0},
                        std::array<uint8_t, 4>{0, 42, 100, 0}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{{100, 42}}),
        // STORE Relative: Store A(42) at address D(90)+10=100
        std::make_tuple(std::array<uint8_t, 4>{42, 0, 0, 90}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::RELATIVE, Opcode::STORE,
                                    RegisterId::D, RegisterId::A,
                                    RegisterId::NONE, 10, 0},
                        std::array<uint8_t, 4>{42, 0, 0, 90}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{{100, 42}}),
        // MOV: Copy B(42) to C
        std::make_tuple(std::array<uint8_t, 4>{0, 42, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::NONE, Opcode::STORE,
                                    RegisterId::B, RegisterId::NONE,
                                    RegisterId::C, 0, 0},
                        std::array<uint8_t, 4>{0, 42, 42, 0}, 0, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{})));

INSTANTIATE_TEST_SUITE_P(
    JUMP, CpuExecuteTest,
    ::testing::Values(
        // JUMP Immediate: Jump to address 42
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::JUMP,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 42, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // JUMP Register: Jump to address in B(42)
        std::make_tuple(std::array<uint8_t, 4>{0, 42, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::REGISTER, Opcode::JUMP,
                                    RegisterId::B, RegisterId::NONE,
                                    RegisterId::NONE, 0, 0},
                        std::array<uint8_t, 4>{0, 42, 0, 0}, 42, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // JUMP Relative: Jump to PC+10=10
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 5, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::RELATIVE, Opcode::JUMP,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 10, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 15, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // HALT: Halt the Cpu
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::NONE, Opcode::JUMP,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 0, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0, true,
                        std::vector<std::pair<uint8_t, uint8_t>>{})));

INSTANTIATE_TEST_SUITE_P(
    JUMPZ, CpuExecuteTest,
    ::testing::Values(
        // JUMPZ Immediate when Z flag is set: Jump to 42
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b01,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::JUMPZ,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 42, 0b01, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // JUMPZ Immediate when Z flag is not set: Don't jump
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b00,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::JUMPZ,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b00, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{})));

INSTANTIATE_TEST_SUITE_P(
    JUMPNZ, CpuExecuteTest,
    ::testing::Values(
        // JUMPNZ Immediate when Z flag is not set: Jump to 42
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b00,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE,
                                    Opcode::JUMPNZ,  // Instruction: JUMPNZ #42
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 42, 0, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // JUMPNZ Immediate when Z flag is set: Don't jump
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b01,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::JUMPNZ,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b01, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{})));

INSTANTIATE_TEST_SUITE_P(
    JUMPN, CpuExecuteTest,
    ::testing::Values(
        // JUMPN Immediate when N flag is set: Jump to 42
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b10,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::JUMPN,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 42, 0b10, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{}),
        // JUMPN Immediate when N flag is not set: Don't jump
        std::make_tuple(std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b01,
                        std::vector<std::pair<uint8_t, uint8_t>>{},
                        Instruction{AddressingMode::IMMEDIATE, Opcode::JUMPN,
                                    RegisterId::NONE, RegisterId::NONE,
                                    RegisterId::NONE, 42, 0},
                        std::array<uint8_t, 4>{0, 0, 0, 0}, 0, 0b01, false,
                        std::vector<std::pair<uint8_t, uint8_t>>{})));

TEST(CpuFetchTest, FetchInstruction) {
  Cpu cpu;
  Memory memory;

  memory.WriteByte(0, 0b01111111);
  memory.WriteByte(1, 0b10011111);

  cpu.Fetch(memory);

  EXPECT_EQ(cpu.GetIr(), 0b0111111110011111);
  EXPECT_EQ(cpu.GetPc(), 2);
}

class CpuCalculateLoadStoreOffsetTest
    : public ::testing::TestWithParam<Opcode> {};

TEST_P(CpuCalculateLoadStoreOffsetTest, CalculatePositiveOffset) {
  const uint16_t immediate = 0b01010101;
  // NOLINTNEXTLINE(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
  const int8_t offset = Cpu::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, 85);
}

TEST_P(CpuCalculateLoadStoreOffsetTest, CalculateNegativeOffset) {
  const uint16_t immediate = 0b11010101;
  // NOLINTNEXTLINE(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
  const int8_t offset = Cpu::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, -43);
}

TEST_P(CpuCalculateLoadStoreOffsetTest, CalculateZeroOffset) {
  const uint16_t immediate = 0b00000000;
  // NOLINTNEXTLINE(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
  const int8_t offset = Cpu::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, 0);
}

INSTANTIATE_TEST_SUITE_P(LOADSTORE, CpuCalculateLoadStoreOffsetTest,
                         ::testing::Values(Opcode::LOAD, Opcode::STORE));

class CpuCalculateJumpOffsetTest : public ::testing::TestWithParam<Opcode> {};

TEST_P(CpuCalculateJumpOffsetTest, CalculatePositiveOffset) {
  const uint16_t immediate = 0b010000010;
  const int16_t offset = Cpu::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, 130);
}

TEST_P(CpuCalculateJumpOffsetTest, CalculateNegativeOffset) {
  const uint16_t immediate = 0b100000110;
  const int16_t offset = Cpu::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, -250);
}

TEST_P(CpuCalculateJumpOffsetTest, CalculateZeroOffset) {
  const uint16_t immediate = 0b000000000;
  const int16_t offset = Cpu::CalculateOffset(immediate, GetParam());
  EXPECT_EQ(offset, 0);
}

INSTANTIATE_TEST_SUITE_P(JUMP, CpuCalculateJumpOffsetTest,
                         ::testing::Values(Opcode::JUMP, Opcode::JUMPZ,
                                           Opcode::JUMPNZ, Opcode::JUMPN));

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
