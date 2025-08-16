# Sample Program

This sample program counts down from 5, subtracting 1 each CPU cycle.

```
10 05 : Load value at address 16 into register A
11 45 : Load value at address 17 into register B
00 42 : Subtract value in register B from value in register A and store the result in register A
FE 1D : Jump to subtraction instruction if result was non-zero
12 07 : Store value in register A at address 18 (will store 0)
FF 08 : Halt program
00 00 : Padding for code segment
00 00 : Padding for code segment
05 01 : Values 5 and 1
```
