[BITS 16]
[ORG 0xF000]
jmp loaderMain

;GDT
gdt_begin:
; Null segment, reserved by CPU
gdt_null:
    dd 0x00000000
    dd 0x00000000

; Code segment
gdt_code:
    ; Segment limit (4 GiB)
    dw 0xFFFF

    ; Segment base address (16 bits)
    dw 0x0000

    ; Segment base address (8 bits)
    db 0x00

    ; 1 - present bit (1 for all valid sectors)
    ; 00 - privilege (ring level), 00 is the higest
    ; 1 - reserved
    ; 1 - excebutable bit, code can be excecuted here
    ; 0 - conforming bit, only kernel can execute code
    ; 1 - segment can be read
    ; 0 - access bit, default is zero
    db 10011010b

    ; 1 - granularity (0 = 1B block, 1 = 4 KiB block)
    ; 1 - size bit (0 = 16b protected mode, 1 = 32b protected mode)
    ; 00 - reserved
    ; 1111 - segment base address (4 bits)
    db 11001111b

    ; Segment base address (8 bits)
    db 0x00

; Data segment
gdt_data:
    ; Segment limit (4 GiB)
    dw 0xFFFF

    ; Segment base address (16 bits)
    dw 0x0000

    ; Segment base address (8 bits)
    db 0x00

    ; 1 - present bit (1 for all valid sectors)
    ; 00 - privilege (ring level), 00 is the higest
    ; 1 - reserved
    ; 0 - excebutable bit, code can't be excecuted here (because it's just data)
    ; 0 - conforming bit, only kernel can execute code
    ; 1 - segment can be read
    ; 0 - access bit, default is zero
    db 10010010b

    ; 1 - granularity (0 = 1B block, 1 = 4 KiB block)
    ; 1 - size bit (0 = 16b protected mode, 1 = 32b protected mode)
    ; 00 - reserved
    ; 1111 - segment base address (4 bits)
    db 11001111b

    ; Segment base address (8 bits)
    db 0x00
gdt_end:
gdt_description:
    dw gdt_end - gdt_begin - 1
    dd gdt_begin


;Globals
micros_loading db 'MicrOS is loading...',0
a20_error db 'A20 Gate is not available. Critical ERROR...',0
before_prot db 'This is exactly before turning on protection!',0

loaderMain:
    cli

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x7C00

    ; Hide cursor
    mov ah, 0x01
    mov cx, 0x2607
    int 0x10

    ; Move cursor at top left position
    mov ah, 0x02
    xor bx, bx
    xor dx, dx
    int 0x10

    ; Clear screen
    mov ah, 0x06
    xor al, al
    xor bx, bx
    mov bh, 0x07
    xor cx, cx
    mov dh, 24
    mov dl, 79
    int 0x10

    mov si, micros_loading
    call print_line_16

    ; Check If Line A20 Enabled
    call check_a20
    or ax, ax
    jz enable_A20_BIOS

    cli

    mov bx, 0x0000
    mov es, bx
    ; Buffer offset
    mov bx, 0x5C00
    mov di, bx

    call load_memory_map

    sti
    ; Load GDT table
    lgdt [dword gdt_description]



    ;enter protected mode (32 bit)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword 0x08:main_protected_area
    jmp $

print_line_16:
	mov ah, 0Eh
print_line_16_repeat:
	lodsb
    test al, al
	je print_line_16_done
	int 10h
	jmp print_line_16_repeat
print_line_16_done:
    call new_line_16
    ret

new_line_16:
    mov ah, 0Eh
    mov al, 0Ah
    int 10h
    mov al, 0Dh
    int 10h
    ret

check_a20:
    pushf
    push ds
    push es
    push di
    push si
    cli
    xor ax, ax ; ax = 0
    mov es, ax
    not ax ; ax = 0xFFFF
    mov ds, ax
    mov di, 0x0500
    mov si, 0x0510
    mov al, byte [es:di]
    push ax
    mov al, byte [ds:si]
    push ax
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
    cmp byte [es:di], 0xFF
    pop ax
    mov byte [ds:si], al
    pop ax
    mov byte [es:di], al
    mov ax, 0
    je check_a20__exit
    mov ax, 1
check_a20__exit:
    pop si
    pop di
    pop es
    pop ds
    popf
    ret

halt_a20:
    mov si, a20_error
    call print_line_16
    cli
    hlt

enable_A20_BIOS:
    mov     ax,2403h                ;--- A20-Gate Support ---
    int     15h
    jb      a20_bios_failed                  ;INT 15h is not supported
    cmp     ah,0
    jnz     a20_bios_failed                  ;INT 15h is not supported
    mov     ax,2402h                ;--- A20-Gate Status ---
    int     15h
    jb      a20_bios_failed             ;couldn't get status
    cmp     ah,0
    jnz     a20_bios_failed              ;couldn't get status
    cmp     al,1
    jz      a20_enabled           ;A20 is already activated
    mov     ax,2401h                ;--- A20-Gate Activate ---
    int     15h
    jb      a20_bios_failed              ;couldn't activate the gate
    cmp     ah,0
    jnz     a20_bios_failed              ;couldn't activate the gate
    jmp     a20_check_after_bios
enable_A20_keyboard:
    cli
    call    a20wait
    mov     al,0xAD
    out     0x64,al
    call    a20wait
    mov     al,0xD0
    out     0x64,al
    call    a20wait2
    in      al,0x60
    push    eax
    call    a20wait
    mov     al,0xD1
    out     0x64,al
    call    a20wait
    pop     eax
    or      al,2
    out     0x60,al
    call    a20wait
    mov     al,0xAE
    out     0x64,al
    call    a20wait
    sti
    ret

a20wait:
    in      al,0x64
    test    al,2
    jnz     a20wait
    ret
 
a20wait2:
    in      al,0x64
    test    al,1
    jz      a20wait2
    ret


; Check if Enabled via BIOS
a20_check_after_bios:
    call check_a20
    or ax, ax
    jnz a20_enabled
; Not Enabled via BIOS. Try by Keyboard Controller
a20_bios_failed:
    call enable_A20_keyboard
; Check if Enabled via Keyboard Controller
a20_keyboard_check:
    call check_a20
    or ax, ax
    jnz a20_enabled
; Not Enabled via Keyboard Controller, Try Fast A20
a20_keyboard_failed:
    in al, 0x92
    or al, 2
    out 0x92, al
; Check if enabled via Fast A20
a20_fast_a20_check:
    call check_a20
    or ax, ax
    jnz a20_enabled
; Not Enabled via A20. No idea what to do here
    mov si, a20_error
    call print_line_16
    cli
    hlt
; Enabled Line A20
a20_enabled:
    ; Jump to protected area 
    ret

; Input:
;   es - buffer segment
;   di - buffer offset
load_memory_map:
    xor ebx, ebx

    ; Push initial buffer address
    push di

    ; Push initial entries count
    mov eax, 0
    push eax

    load_memory_map_loop:
    ; Increment pointer in buffer
    add di, 24

    ; Increment entries count
    pop eax
    inc eax
    push eax

    ; Set magic number
    mov edx, 0x534d4150

    ; Set number of bytes to read
    mov ecx, 24

    ; Read memory map
    mov eax, 0xe820
    int 0x15

    ; Disable interrupts (can be enabled by int 0x15)
    cli

    ; Exit if ebx is equal to zero (reading has ended)
    cmp ebx, 0
    jne load_memory_map_loop

    ; Store entries count at the begin of the buffer
    pop eax
    pop di
    mov [di], eax
    
    ret

[BITS 32]
main_protected_area:
    ;call protstr
    cli
    ; Set data and stack segments to the third GDI descriptor
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esi, protected_in
    call protstr
    
    hlt
    jmp $

; temporarily it is going to print at the beginning!
; Function for displaying debug text in protected mode
protstr:
    mov dx,0x3D4       ; Tell the control I/O port to get the lower byte of
    mov al,0x0F        ; the cursor offset
    out dx,al
    mov dx,0x3D5       ; Switch to data I/O port
    in  al,dx          ; Get the cursor offset's lower byte

    mov dx,0x3D5       ; Tell the control I/O port to get the higher byte of
    mov al,0x0E        ; the cursor offset
    out dx,al
    mov dx,0x3D5       ; Switch to data I/O port
    in  al,dx          ; Get the higher byte

    imul ax,2
    ;discard for now
    mov ah, 09
    mov ebx, 0xb8000
protstr_loop:
	lodsb
    test al, al
    je protstr_done
    mov [ebx], ax
    add ebx, 2
    jmp protstr_loop
protstr_done:
    ret

; 32bit globals
protected_in db 'We are in protected mode now! Second stage worked!',0