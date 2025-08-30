#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include "config.hpp"

constexpr size_t BANK_SIZE = 256;

class Memory {
 private:
  uint8_t curr_bank;
  std::vector<std::array<uint8_t, BANK_SIZE>> banks;
  uint8_t num_banks;

 public:
  Memory()
      : num_banks{Config::DEFAULT_NUM_BANKS},
        banks{Config::DEFAULT_NUM_BANKS},
        curr_bank{0} {}
  Memory(uint8_t num_banks)
      : num_banks{num_banks}, banks{num_banks}, curr_bank{0} {}

  [[nodiscard]] uint8_t GetCurrentBank() const noexcept;
  [[nodiscard]] uint8_t GetNumBanks() const noexcept;
  [[nodiscard]] uint8_t ReadByte(const uint8_t addr) const noexcept;
  void SetCurrentBank(const uint8_t bank) noexcept;
  void WriteByte(const uint8_t addr, const uint8_t val) noexcept;

  friend std::ostream& operator<<(std::ostream& os, const Memory& mem);
};

#endif
