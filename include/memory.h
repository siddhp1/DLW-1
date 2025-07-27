#ifndef MEMORY_H
#define MEMORY_H

#include <sys/types.h>

#include <array>

class Memory {
 private:
  std::array<u_int8_t, 256> mem = {0};

 public:
  // accessors and mutators for memory
  void write8(u_int8_t addr, u_int8_t val);

  void print();
};

#endif
