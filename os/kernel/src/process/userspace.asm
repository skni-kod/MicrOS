global enter_user_space

; Input: interrupt state on stack
; Output: nothing
enter_user_space:
    ; Set data segments (0x20 + 0x03 (ring 3))
    mov ax, 0x23
    mov ds, ax
    mov es, ax 
    mov fs, ax 
    mov gs, ax

    ; Set stack pointer to the interrupt state structure
    mov eax, [esp + 4]
    mov esp, eax
    
    ; Restore registers
    popa
    
    ; Load FPU state
    frstor [esp]
    fwait
    
    ; Move stack pointer (frstor won't do this itself)
    add esp, 108
    
    ; Skip interrupt number
    add esp, 4
    
    ; Bye!
    iret