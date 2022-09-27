[BITS 16]
mov ax, 5650h
int 1ah

cmp ax, 564Eh
jne bad

mov ah, 13h
mov al, 01h
mov cx, 6h
mov bp,bx
mov bl, 0xc
mov bh, 0x0
mov dx, 0x0

int 0x10

mov bx, bp
mov ax, [es:bx+0x6]
cmp ax, 0x0201
jge pxe

jmp end

pxe:
mov ah, 0xa
mov al, 'P'
mov bh, 0
mov cx, 10
int 0x10
jmp end

bad:
mov ah, 0xa
mov al, 'b'
mov bh, 0
mov cx, 10
int 0x10

end:
cli
hlt