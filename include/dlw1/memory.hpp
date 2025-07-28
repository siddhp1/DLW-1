#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <cstdint>

class Memory {
 private:
  std::array<uint8_t, 256> mem = {0};

 public:
  // accessors and mutators for memory
  void write8(uint8_t addr, uint8_t val);

  void print();
};

#endif
