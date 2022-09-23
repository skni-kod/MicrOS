[BITS 16]
;[ORG 0x7C00]
mov ah, 0xa
mov al, 'a'
mov bh, 0
mov cx, 10
int 0x10