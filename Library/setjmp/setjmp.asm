global setjmp

setjmp:
    mov eax, [esp + 4]
    mov [eax + 0], ebx  ; evn[0] = ebx
    mov [eax + 4], ecx  ; evn[1] = ecx
    mov [eax + 8], edx  ; evn[2] = edx
    mov [eax + 12], esi ; evn[3] = esi
    mov [eax + 16], edi ; evn[4] = edi
    mov [eax + 20], esp ; evn[5] = esp
    mov [eax + 24], ebp ; evn[6] = ebp
    
    mov ebx, [esp]
    mov [eax + 28], ebx ; env[7] = target eip
    
    xor eax, eax
    ret