# DLW-1 Instruction Formats

## Binary Encoding of Instructions

A complete machine language instruction is 16 bits (2 bytes).

### Arithmetic Register Format
| Bit        | 0   | 1-3   | 4-5  | 6-7  | 8-9  | 10-15  |
|------------|-----|-------|------|------|------|--------|
| Field      | mode| opcode| src1 | src2 | dest | empty  |
| Value      | 0   | 000/001| reg | reg  | reg  | 000000 |

### Arithmetic Immediate Format
| Bit        | 0   | 1-3   | 4-5  | 6-7  | 8-15         |
|------------|-----|-------|------|------|--------------|
| Field      | mode| opcode| src1 | src2 | immediate value |
| Value      | 1   | 000/001| reg | reg  | imm8         |

### Immediate Load Format
| Bit        | 0   | 1-3   | 4-5   | 6-7  | 8-15            |
|------------|-----|-------|-------|------|-----------------|
| Field      | mode| opcode| unused| dest | immediate address|
| Value      | 1   | 010   | 00    | reg  | addr8           |

### Register Load Format
| Bit        | 0   | 1-3   | 4-5  | 6-7   | 8-9  | 10-15  |
|------------|-----|-------|------|-------|------|--------|
| Field      | mode| opcode| src1 | unused| dest | empty  |
| Value      | 0   | 010   | reg  | 00    | reg  | 000000 |

### Relative Load Format
| Bit        | 0   | 1-3   | 4-5  | 6-7  | 8-15   |
|------------|-----|-------|------|------|--------|
| Field      | mode| opcode| base | dest | offset  |
| Value      | 1   | 010   | reg  | reg  | offset8 |

### Immediate Store Format
| Bit        | 0   | 1-3   | 4-5  | 6-7   | 8-15            |
|------------|-----|-------|------|-------|-----------------|
| Field      | mode| opcode| src  | unused| immediate address|
| Value      | 1   | 011   | reg  | 00    | addr8           |

### Register Store Format
| Bit        | 0   | 1-3   | 4-5  | 6-7   | 8-9  | 10-15  |
|------------|-----|-------|------|-------|------|--------|
| Field      | mode| opcode| src  | unused| dest | empty  |
| Value      | 0   | 011   | reg  | 00    | reg  | 000000 |

### Relative Store Format
| Bit        | 0   | 1-3   | 4-5  | 6-7  | 8-15   |
|------------|-----|-------|------|------|--------|
| Field      | mode| opcode| src  | base | offset  |
| Value      | 1   | 011   | reg  | reg  | offset8 |
