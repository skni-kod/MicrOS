global longjmp

; Input:
;   - first stack parameter: array of 5 32-bit integers which will be used to restore registers
;   - second stack parameter: return value
; Output: 0 if this is normall call or eax if setjmp has been called by longjmp function
longjmp:
    mov eax, [esp + 8]  ; Load return value to eax
    mov edx, [esp + 4]  ; Load array of register values to edx
    
    mov ebx, [edx]      ; evn[0] = ebx
    mov esi, [edx + 4]  ; evn[1] = esi
    mov edi, [edx + 8]  ; evn[2] = edi
    mov esp, [edx + 12] ; evn[3] = eep
    add esp, dword 4    ; add 4 bytes (skip return address which is already on the stack)
    
    jmp [edx + 16]      ; evn[0] = ebx

    ret