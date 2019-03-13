global setjmp

setjmp:
    mov [esp + 4], ebx  ; evn[0] = ebx
    mov [eax + 8], ecx  ; evn[1] = ecx
    mov [eax + 12], edx ; evn[2] = edx
    mov [eax + 16], esi ; evn[3] = esi
    mov [eax + 20], edi ; evn[4] = edi
    mov [eax + 24], esp ; evn[5] = esp
    mov [eax + 28], ebp ; evn[6] = ebp
    
    mov eax, [esp]
    mov [esp + 32], eax ; env[7] = target eip
    
    xor eax, eax
    ret