global setjmp

setjmp:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 4]
    mov [ebp + 8], eax
    mov [eax + 0], ebx  ; evn[0] = ebx
    mov [eax + 4], ecx  ; evn[1] = ecx
    mov [eax + 8], edx  ; evn[2] = edx
    mov [eax + 12], esi ; evn[3] = esi
    mov [eax + 16], edi ; evn[4] = edi
    mov [eax + 20], esp ; evn[5] = esp
    mov [eax + 24], ebp ; evn[6] = ebp
    
    push ebx
    mov ebx, [ebp + 4]
    mov [eax + 28], ebx ; env[7] = target eip
    pop ebx
    
    xor eax, eax
    
    mov esp, ebp
    pop ebp
    
    ret