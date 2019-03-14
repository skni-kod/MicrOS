global longjmp

longjmp:
    mov eax, [esp + 8]
    mov edx, [esp + 4]
    
    mov ebx, [edx]
    mov esi, [edx + 4]
    mov edi, [edx + 8]
    mov esp, [edx + 12]
    add esp, dword 4
    
    jmp [edx + 16]

    ret