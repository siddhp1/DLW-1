# Sample Program

This sample program counts down from 5, subtracting 1 each CPU cycle.

### Machine code:
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

### Assembly:
```
		.org 0x00
start:	load ra, #0x10	; Load value at address 16 into register a
		load rb, #0x11	; Load value at address 17 into register b

loop:	sub ra, rb, ra	; Register a = register a - register b
		jumpnz loop		; Jump to loop label if not zero

		store ra, #0x12	; Store register a contents at address 18
		halt			; End program

		.org 0x10
		.byte 5			; Counter start
		.byte 1			; Decrement step
```
