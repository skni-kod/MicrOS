[BITS 16]
[ORG 0x0000]

jmp Main

HelloWorldString        db 'Hello, World!', 0

Main:
    mov si, HelloWorldString
    call PrintString

JMP $

; Input:
;   si - address of the string ended with null char
; Output: nothing
PrintString:
    lodsb
    
    or al, al
    jz PrintString_End
    
    ; Function name: Teletype Output
    mov ah, 0x0E

    ; Page number and color to 0
    mov bx, 0

    int 0x10
    
    jmp PrintString

    PrintString_End:
    ret