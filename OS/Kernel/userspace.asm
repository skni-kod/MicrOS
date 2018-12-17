global enter_user_space

enter_user_space:
    mov eax, esp
    mov ebx, [esp + 40]
    mov ecx, [esp + 56]
    mov edx, [esp + 52]

    push dword ecx
    push dword eax
    pushf
    push dword edx
    push dword ebx

    mov ax, [esp + 20 + 56]
    mov ds, ax
    mov es, ax 
    mov fs, ax 
    mov gs, ax

    mov eax, [esp + 20 + 4]
    mov ebx, [esp + 20 + 8]
    mov ecx, [esp + 20 + 12]
    mov edx, [esp + 20 + 16]
    mov ebp, [esp + 20 + 24]
    mov esi, [esp + 20 + 28]
    mov edi, [esp + 20 + 32]
    
    iret