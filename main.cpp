#include <iostream>

enum class Opcode : u_int8_t {
    ADD = 0b00,
    SUB = 0b001,
    LOAD = 0b010,
    STORE = 0b011,
    JUMP = 0b100,
    JUMPZ = 0b101,
    JUMPNZ = 0b110,
    JUMPN = 0b111,
};

enum class RegisterID : u_int8_t {
    A = 0b00,
    B = 0b01,
    C = 0b10,
    D = 0b11,
};

enum class InstructionMode : u_int8_t {
    REGISTER = 0b0,
    IMMEDIATE = 0b1,
};

struct CPU {
    u_int8_t registers[4];
    u_int8_t memory[256];
    u_int8_t pc = 0;
    u_int16_t ir = 0;
    u_int8_t psw : 4 = 0; // Only need 4 bits
};

int main(void)
{
    return 0;
}
