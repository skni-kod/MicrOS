[BITS 32]
[ORG 0x0000]

Main:
   mov byte [0xb8000], 'X'

JMP $