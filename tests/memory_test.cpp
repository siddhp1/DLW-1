#include "dlw1/memory.hpp"

#include "gtest/gtest.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST(MemoryReadWriteTest, ReadWriteSameBank) {
  Memory memory;
  EXPECT_EQ(memory.GetCurrentBank(), 0);

  memory.WriteByte(128, 25);
  EXPECT_EQ(memory.ReadByte(128), 25);
}

TEST(MemoryReadWriteTest, ReadWriteDifferentBanks) {
  Memory memory{2};
  EXPECT_EQ(memory.GetCurrentBank(), 0);

  memory.WriteByte(128, 25);
  memory.SetCurrentBank(1);

  memory.WriteByte(128, 50);
  EXPECT_EQ(memory.ReadByte(128), 50);

  memory.SetCurrentBank(0);
  EXPECT_EQ(memory.ReadByte(128), 25);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
