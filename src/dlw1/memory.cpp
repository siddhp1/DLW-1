#include "dlw1/memory.hpp"

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

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
  const std::size_t matrix_rows = 16;
  const std::size_t matrix_cols = 16;
  const std::size_t total_width = (matrix_cols * 3) + 1;

  for (std::size_t bank = 0; bank < mem.GetNumBanks(); ++bank) {
    // Top border with bank number centered
    const std::string bank_header = " Bank " + std::to_string(bank) + " ";
    std::string border_line = "+";
    const std::size_t dashes_total = (total_width > bank_header.size())
                                         ? total_width - bank_header.size()
                                         : 0;
    const std::size_t left_dashes = dashes_total / 2;
    const std::size_t right_dashes = dashes_total - left_dashes;
    border_line.append(std::string(left_dashes, '-'));
    border_line.append(bank_header);
    border_line.append(std::string(right_dashes, '-'));
    border_line.append("+\n");
    os << border_line;

    for (std::size_t row = 0; row < matrix_rows; ++row) {
      os << "| ";
      for (std::size_t col = 0; col < matrix_cols; ++col) {
        auto addr = static_cast<uint8_t>((row * matrix_cols) + col);
        // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
        os << std::setw(2) << std::setfill('0') << std::uppercase << std::hex
           << static_cast<int>(mem.banks[bank][addr]);
        // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
        if (col < matrix_cols - 1) {
          os << " ";
        }
      }
      os << " |\n";
    }

    // Bottom border
    os << "+";
    for (std::size_t x = 0; x < total_width; ++x) {
      os << "-";
    }
    os << "+\n";

    // Spacing for following banks
    if (bank < mem.GetNumBanks() - 1) {
      os << "\n";
    }
  }

  return os;
}
