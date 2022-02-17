[BITS 16]
[ORG 0x7C00]
msg dw  'HelloWorld!'

call clear_screen
call print_hello_msg

mov ax, 0x5650
int 0x1A
 
mov bp, bx
; video page number.
mov bh, 0 
; attribute(7 is light gray).
mov bl, 0x2
; write mode: character only, cursor moved.
mov al, 1
; string length, hardcoded.
mov cx, 6
; y coordinate
mov dh, 1
; x coordinate
mov dl, 0

mov ah, 0x13
int 0x10

mov ax, [es:bp+0x6]

jmp $

print_hello_msg:
    ; video page number.
    mov bh, 0     
    ; ES:BP is the pointer to string.
    mov ax, 0x0
    mov es, ax    
    mov bp, msg   

    ; attribute(7 is light gray).
    mov bl, 0x2
    ; write mode: character only, cursor moved.
    mov al, 1
    ; string length, hardcoded.
    mov cx, 11
    ; y coordinate
    mov dh, 0
    ; x coordinate
    mov dl, 0

    mov ah, 0x13
    int 0x10
    ret

clear_screen:
    pusha
    mov ah, 0x00
    mov al, 0x03 
    int 0x10
    popa
    ret