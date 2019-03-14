global longjmp

longjmp:
    mov eax, [esp + 4]
    mov ebx, [eax + 0]
    mov ecx, [eax + 4]
    mov edx, [eax + 8]
    mov esi, [eax + 12]
    mov edi, [eax + 16]
    mov esp, [eax + 20]
    mov ebp, [eax + 24]
    
    mov eax, [eax + 28]
    mov [ebp + 4], eax

    xor eax, eax
    ret