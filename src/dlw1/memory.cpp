#include "dlw1/memory.hpp"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <stdexcept>

uint8_t Memory::GetCurrentBank() const { return current_bank; }

uint8_t Memory::GetNumBanks() const { return DEFAULT_NUM_BANKS; }

uint8_t Memory::ReadByte(const uint8_t addr) const {
  // No out of bounds check since addr is always <= bank size
  // Logging statement
  return banks[current_bank][addr];
}

void Memory::SetCurrentBank(const uint8_t bank) { current_bank = bank; }

void Memory::WriteByte(const uint8_t addr, const uint8_t val) {
  // No out of bounds check since addr is always <= bank size
  // Logging statement
  banks[current_bank][addr] = val;
}

std::ostream& operator<<(std::ostream& os, const Memory& mem) {
  // For now, output a 16x16 grid of hex values from the current bank
  os << std::hex << std::setfill('0');

  for (size_t row = 0; row < 16; ++row) {
    for (size_t col = 0; col < 16; ++col) {
      uint8_t addr = row * 16 + col;
      os << std::setw(2) << static_cast<int>(mem.banks[mem.current_bank][addr])
         << std::endl;
    }
    os << std::endl;
  }

  return os;
}
