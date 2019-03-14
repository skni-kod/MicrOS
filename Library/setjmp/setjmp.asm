global setjmp

setjmp:
    mov eax, [esp + 4]
    mov [eax], ebx      ; evn[0] = ebx
    mov [eax + 4], esi  ; evn[1] = esi
    mov [eax + 8], edi  ; evn[2] = edi
    mov [eax + 12], esp ; evn[3] = esp
    
    mov ecx, [esp]
    mov [eax + 16], ecx ; env[4] = target eip
    
    xor eax, eax
    ret