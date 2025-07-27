#include "memory.h"

#include <bitset>
#include <iostream>
#include <stdexcept>

void Memory::write8(u_int8_t addr, u_int8_t val) {
  if (addr >= mem.size()) {
    throw std::out_of_range("Memory write out of bounds");
  }

  // Unsafe write since bounds checking done above
  mem[addr] = val;
}

void Memory::print() {
  bool empty = true;

  for (size_t i = 0; i < mem.size(); ++i) {
    u_int8_t byte = mem[i];

    if (byte != 0) {
      std::cout << "Address " << static_cast<int>(i) << ": ";
      std::cout << std::bitset<8>(byte) << std::endl;
      empty = false;
    }
  }

  if (empty) {
    std::cout << "Memory is empty" << std::endl;
  }
}
