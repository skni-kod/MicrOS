;
; |------------|---------|------------|------------|----------------|--------------|--------------|------------|---------|---------|------------|------------|------------|------------|
; |   1 KiB    |  256 B  |   21 KiB   |   1 KiB    |     4 KiB      |    3 KiB     |    512 B     |   28 KiB   | 578 KiB |  1 KiB  |  383 KiB   |   15 MiB   |   1 MiB    |   4 MiB    |
; | Interrupts |   BDA   | Floppy DMA | Memory map | Page Directory | BootloaderS2 | BootloaderS1 | FAT32 data |  DataS2 |  EBDA   | Video, ROM |   Kernel   |   Stack    | Page Table |
; |            |         |            |            |                |              |              |            |         |         |            |            |            |            |
; |  0x00000   | 0x00400 |  0x00500   |  0x05C00   |    0x06000     |    0x07000   |   0x07C00    |  0x07E00   | 0x0F000 | 0x9FC00 |  0xA0000   | 0x00100000 | 0x01000000 | 0x01100000 |
; |  0x003FF   | 0x004FF |  0x05BFF   |  0x05FFF   |    0x06FFF     |    0x07BFF   |   0x07DFF    |  0x0EFFF   | 0x9FBDD | 0x9FFFF |  0xFFFFF   | 0x00FFFFFF | 0x010FFFFF | 0x014FFFFF |
; |------------|---------|------------|------------|----------------|--------------|--------------|------------|---------|---------|------------|------------|------------|------------|

[BITS 16]
jmp loader

;GDT
gdt_begin:
; null segment, reserved by CPU
gdt_null:
    dd 0x00000000
    dd 0x00000000

; code segment 32 bit (0x08)
gdt_code_32:
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

; data segment 32 bit (0x10)
gdt_data_32:
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
; code segment 16 bit (0x18)
gdt_code_16:
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
    db 00000001b

    ; Segment base address (8 bits)
    db 0x00
; data segment 16 bit (0x20)
gdt_data_16:
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
    db 10010010b

    ; 1 - granularity (0 = 1B block, 1 = 4 KiB block)
    ; 1 - size bit (0 = 16b protected mode, 1 = 32b protected mode)
    ; 00 - reserved
    ; 1111 - segment base address (4 bits)
    db 00000001b

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
protected_in db 'We are in protected mode now! Second stage worked!',0

r_bios db 'BIOS',0
r_keyb db 'KEYBOARD',0
r_gate db 'GATE',0

RealMode.StackPointer     dw 0
RealMode.IntNumber        dw 0

struc register_state
	.eax: resd 1
	.ecx: resd 1
	.edx: resd 1
	.ebx: resd 1
	.esp: resd 1
	.ebp: resd 1
	.esi: resd 1
	.edi: resd 1
	.efl: resd 1

	.es: resw 1
	.ds: resw 1
	.fs: resw 1
	.gs: resw 1
	.ss: resw 1
endstruc


loader:
    cli

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

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

    mov esp, 0x7c00
    
    ; Print "Micros is loading..."
    mov si, micros_loading
    call print_line_16

    ; Check If Line A20 Enabled
    call check_a20
    or ax, ax

    jnz A20Ready

    call enable_A20_BIOS

A20Ready:

    sti
    ; Load GDT table
    lgdt [dword gdt_description]
    cli

    mov bx, 0x0000
    mov es, bx
    ;Buffer offset
    mov bx, 0x5C00
    mov di, bx 
    call load_memory_map

    ;enter protected mode (32 bit)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:jump_to_loader

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

halt_a20:
    mov si, a20_error
    call print_line_16
    cli
    hlt

enable_A20_BIOS:
    mov     ax,2403h                ;--- A20-Gate Support ---
    int     15h
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
    ret

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
jump_to_loader:
    extern loader_main
    call loader_main
    global _pxecall
_pxecall:
	pushad
    jmp 0x018:.foop

[bits 16]
.foop:
    mov ax, 0x20
	mov es, ax
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    
	; Disable protected mode
    mov eax, cr0
    and eax, ~(1 << 0)
    mov cr0, eax

	; Clear all segments
	xor ax, ax
	mov es, ax
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
       
	; Perform a long jump to real-mode
	pushfd                 ; eflags
	push dword 0x0         ; cs
	push dword .new_func   ; eip
	iretd

.new_func:
	;   pxecall(seg: u16, off: u16, pxe_call: u16, param_seg: u16, param_off: u16);
	movzx eax, word [esp + (4*0x9)] ; arg1, seg
	movzx ebx, word [esp + (4*0xa)] ; arg2, offset
	movzx ecx, word [esp + (4*0xb)] ; arg3, pxe_call
	movzx edx, word [esp + (4*0xc)] ; arg4, param_seg
	movzx esi, word [esp + (4*0xd)] ; arg5, param_off

	; Set up PXE call parameters (opcode, offset, seg)
	push dx
	push si
	push cx

	; Set up our return address from the far call
	mov bp, .retpoint
	push cs
	push bp

	; Set up a far call via iretw
	pushfw
	push ax
	push bx

	iretw
.retpoint:
	; Hyper-V has been observed to set the interrupt flag in PXE routines. We clear it ASAP.
	cli

    mov bx, ax

    add sp, 6

    mov eax, cr0
    or eax, 1
    mov cr0, eax

	; Jump back to protected mode
	pushfd             ; eflags
	push dword 0x08    ; cs
	push dword backout ; eip
	iretd

[bits 32]
backout:
    mov ax, 0x10
	mov es, ax
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	popad

    xor eax, eax
    mov ax, bx
	ret
    global enter_kernel:
enter_kernel:
    jmp dword 0x08:0x100000
    jmp $