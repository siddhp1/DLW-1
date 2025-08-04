#include "dlw1/memory.hpp"

#include <bitset>
#include <iostream>

uint8_t Memory::GetCurrentBank() const { return curr_bank; }

uint8_t Memory::ReadByte(const uint8_t addr) const {
  // No out of bounds check since addr is always <= bank size
  // Logging statement
  return banks[curr_bank][addr];
}

void Memory::SetCurrentBank(const uint8_t bank) { curr_bank = bank; }

void Memory::WriteByte(const uint8_t addr, const uint8_t val) {
  // No out of bounds check since addr is always <= bank size
  // Logging statement
  banks[curr_bank][addr] = val;
}

std::ostream& operator<<(std::ostream& os, const Memory& mem) {
  // For now, output an 8x32 matrix of binary values
  for (size_t row = 0; row < 32; ++row) {
    for (size_t col = 0; col < 8; ++col) {
      uint8_t addr = row * 8 + col;
      os << std::bitset<8>(mem.banks[mem.curr_bank][addr]) << " ";
    }
    os << std::endl;
  }

  return os;
}
