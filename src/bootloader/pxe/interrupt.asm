; [BITS 16]

; section .text
; GLOBAL interrupt

; interrupt:
;     push bp
;     mov bp, sp
;     push si
;     mov ax, [bp + 0x6]
;     mov bx, [bp + 0x8]
;     mov cx, [bp + 0xa]
;     mov dx, [bp + 0xc]
;     int 0x10