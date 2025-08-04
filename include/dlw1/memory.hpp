#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <cstdint>
#include <iostream>

class Memory {
 private:
  static constexpr size_t BANK_SIZE = 256;
  static constexpr uint8_t DEFAULT_NUM_BANKS = 4;

  // Make the number of banks configurable using a constuctor after
  std::array<std::array<uint8_t, BANK_SIZE>, DEFAULT_NUM_BANKS> banks;
  uint8_t curr_bank;

 public:
  Memory() : banks{}, curr_bank{0} {}

  uint8_t GetCurrentBank() const;
  uint8_t ReadByte(const uint8_t addr) const;
  void SetCurrentBank(const uint8_t bank);
  void WriteByte(const uint8_t addr, const uint8_t val);

  friend std::ostream& operator<<(std::ostream& os, const Memory& mem);
};

#endif
