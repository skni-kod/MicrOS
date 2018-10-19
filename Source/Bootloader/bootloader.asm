[BITS 16]
[ORG 0x7C00]

JMP $

TIMES 510 - ($ - $$) db 0
DW 0xAA55