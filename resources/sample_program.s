        .org 0x00
start:  load ra, #0x10      ; Load value at address 16 into register a
        load rb, #0x11      ; Load value at address 17 into register b

loop:   sub ra, rb, ra      ; Register a = register a - register b
        jumpnz loop         ; Jump to loop label if not zero

        store ra, #0x12     ; Store register a contents at address 18
        halt                ; End program

        .org 0x10
        .byte 5             ; Counter start
        .byte 1             ; Decrement step
