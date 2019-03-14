global longjmp

; Input:
;   - first stack parameter: array of 5 32-bit integers which will be used to restore registers
;   - second stack parameter: return value
; Output: 0 if this is normall call or eax if setjmp has been called by longjmp function
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