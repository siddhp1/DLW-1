# DLW-1 Specification

## Instructions

### Arithmetic Instructions

Format: instruction source1, source2, destination

Instructions:
- add
- subtract

### Memory Instructions

Format: instruction source, destination

Instructions:
- load
- store

### Branch Instructions

Format: instruction destination

Instructions:
- jump (unconditional)
- jumpz (jump if zero)
- jumpnz (jump if not zero)
- jumpn (jump if negative)

### Instruction Mapping

| Mnemonic | Opcode |
|----------|--------|
| add      | 000    |
| sub      | 001    |
| load     | 010    |
| store    | 011    |
| jump     | 100    |
| jumpz    | 101    |
| jumpnz   | 110    |
| jumpn    | 111    |

Notes:
- sources can also be replaced with immediate values
- immediate values can be used as memory addresses (prefixed with #)
- sources can be addressed relative to other memory cells

## Hardware Specification

- 4 4-bit general purpose registers (A, B, C, D) mapped to 2-bit codes:

    | Register | Binary Code |
    |----------|-------------|
    | A        | 00          |
    | B        | 01          |
    | C        | 10          |
    | D        | 11          |

- 256-byte memory
- 4-bit processor status word (PSW)
- 8-bit program counter (PC)
- 16-bit instruction register (IR)
