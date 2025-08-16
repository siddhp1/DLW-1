#include "dlw1/memory.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>

uint8_t Memory::GetCurrentBank() const noexcept { return curr_bank; }

uint8_t Memory::GetNumBanks() const noexcept { return num_banks; }

uint8_t Memory::ReadByte(const uint8_t addr) const noexcept {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
  return banks[curr_bank][addr];
}

void Memory::SetCurrentBank(const uint8_t bank) noexcept { curr_bank = bank; }

void Memory::WriteByte(const uint8_t addr, const uint8_t val) noexcept {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
  banks[curr_bank][addr] = val;
}

std::ostream& operator<<(std::ostream& os, const Memory& mem) {
  const std::size_t matrix_rows = 32;
  const std::size_t matrix_cols = 8;

  for (std::size_t bank = 0; bank < mem.GetNumBanks(); ++bank) {
    for (std::size_t row = 0; row < matrix_rows; ++row) {
      for (std::size_t col = 0; col < matrix_cols; ++col) {
        const uint8_t addr = (row * 8) + col;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,
        // cppcoreguidelines-pro-bounds-constant-array-index,
        // readability-magic-numbers)
        os << std::bitset<8>(mem.banks[mem.curr_bank][addr]) << " ";
      }
      os << "\n";
    }
    os << "\n";
  }

  return os;
}
