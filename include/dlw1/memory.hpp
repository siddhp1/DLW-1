#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <cstdint>
#include <iostream>

class Memory {
 private:
  static constexpr size_t BANK_SIZE = 256;
  static constexpr size_t NUM_BANKS = 4;

  std::array<std::array<uint8_t, BANK_SIZE>, NUM_BANKS> banks;
  uint8_t current_bank = 0;

 public:
  uint8_t ReadByte(const uint8_t addr) const;
  void WriteByte(const uint8_t addr, const uint8_t val);

  friend std::ostream& operator<<(std::ostream& os, const Memory& mem);
};

#endif
