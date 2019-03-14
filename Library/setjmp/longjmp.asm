global longjmp

longjmp:
    mov eax, [esp + 8]
    mov edx, [esp + 4]
    
    mov ebx, [edx + 0]
    mov ecx, [edx + 4]
    ;mov edx, [edx + 8]
    mov esi, [edx + 12]
    mov edi, [edx + 16]
    ;mov esp, [eax + 20]
    ;add esp, dword 12
    ;mov ebp, [eax + 24]
    
    jmp [edx + 28]

    ret