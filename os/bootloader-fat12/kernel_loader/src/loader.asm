[BITS 16]

jmp loaderMain


loaderMain:
    mov ax, 0xF000
    mov ds, ax 

    ; Hide cursor
    mov ah, 0x01
    mov cx, 0x2607
    int 0x10

    ; Move cursor at top left position
    mov ah, 0x02
    xor bx, bx
    xor dx, dx
    int 0x10

    ; Clear screen
    mov ah, 0x06
    xor al, al
    xor bx, bx
    mov bh, 0x07
    xor cx, cx
    mov dh, 24
    mov dl, 79
    int 0x10

    ;halt

    mov si, micros_loading
    call print_line_16
    cli
    hlt

print_line_16:
	mov ah, 0Eh
print_line_16_repeat:
	lodsb
    test al, al
	je print_line_16_done
	int 10h
	jmp print_line_16_repeat
print_line_16_done:
    call new_line_16
    ret

new_line_16:
    mov ah, 0Eh
    mov al, 0Ah
    int 10h
    mov al, 0Dh
    int 10h
    ret

micros_loading db 'MicrOS is loading...',0