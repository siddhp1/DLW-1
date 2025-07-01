#ifndef CPU_H
#define CPU_H

#include <sys/types.h>

#include <array>

// Mapping for general purpose registers
enum class RegisterID : u_int8_t {
  A = 0b00,
  B = 0b01,
  C = 0b10,
  D = 0b11,
};

class CPU {
 private:
  u_int8_t pc = 0;       // 8-bit (1 byte) program counter
  u_int16_t ir = 0;      // 16-bit (2 byte) instruction register
  u_int8_t psw : 4 = 0;  // 4-bit processor status word

  std::array<u_int8_t, 4> gpr = {
      0};  // 4 8-bit (1 byte) general purpose registers

 public:
  // fetch method
  // decode method
  // execute method
  // accessors and mutators where needed
};

#endif
