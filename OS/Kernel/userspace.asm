global enter_user_space

enter_user_space:
    mov ax, 0x23
    mov ds, ax
    mov es, ax 
    mov fs, ax 
    mov gs, ax

    mov eax, [esp + 4]
    mov esp, eax

    popa
    add esp, 4
    
    iret