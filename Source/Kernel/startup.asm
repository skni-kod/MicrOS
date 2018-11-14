;                       MicrOS kernel memory layout in protected mode
;                             Entry 768 in the directory table
; |---------------|------------|----------------|-------------|-------------------------|
; |    1 MiB      |   1 MiB    |      4 KiB     |    4 MiB    |        1017 MiB         |
; | BIOS & Kernel |   Stack    | Page directory | Page tables | Dynamic allocation area |
; |               |            |                |             |                         | 
; |  0xC0000000   | 0xC0100000 |   0xC0200000   | 0xC0201000  |       0xC0601000        |
; |  0xC00FFFFF   | 0xC01FFFFF |   0xC0200FFF   | 0xC0600FFF  |       0xFFFFFFFF        |
; |---------------|------------|----------------|-------------|-------------------------|
; {                       Page 1                       }{   Page 2   }{   Page 3 - n    }

[BITS 16]

jmp Main

; Entry frame: https://wiki.osdev.org/GDT
GDT:
; Null segment, reserved by CPU
GDT_Null:
    dd 0x00000000
    dd 0x00000000

; Code segment
GDT_Code:
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
GDT_Data:
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
GDT_End:
GDT_Desc:
    dw GDT_End - GDT - 1
    dd GDT

Main:
    ; Load memory map before we will switch to the protected mode
    ; Buffer segment
    mov bx, 0x0000
    mov es, bx

    ; Buffer offset
    mov bx, 0x0500
    mov di, bx

    call LoadMemoryMap

    ; Disable interrupts
    cli

    ; Load GDT table
    lgdt [dword GDT_Desc]

    ; Set protected mode flag
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Jump to protected area 
    jmp dword 0x08:Main_ProtectedArea

; Input:
;   es - buffer segment
;   di - buffer offset
LoadMemoryMap:
    xor ebx, ebx

    LoadMemoryMap_Loop:
    ; Set magic number
    mov edx, 0x534d4150

    ; Set number of bytes to read
    mov ecx, 24

    ; Read memory map
    mov eax, 0xe820
    int 0x15

    ; Increment pointer in buffer
    add di, 24

    ; Exit if ebx is equal to zero (reading has ended)
    cmp ebx, 0
    jne LoadMemoryMap_Loop

    ret
    
[BITS 32]
    Main_ProtectedArea:

    ; Set data and stack segments to the third GDI descriptor
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    call CreateDirectoryTable
    call CreateIdentityPageTable
    call CreateKernelPageTable
    call EnablePaging

    ; Set new stack with virtual address
    mov eax, 0xC0200000
    mov esp, eax

    ; Jump to kmain and start kernel work
    extern kmain
    call kmain

    ; Something went wrong, but no problem!
    JMP $

; Input: nothing
; Output: nothing
CreateDirectoryTable:
    ; Add temporary identity entry (physical address: 0, virtual address: 0) to the directory table
    mov eax, 0x00201003
    mov [0x00200000], eax

    ; Add kernel entry (first megabyte will be mapped to 0xc0000000)
    mov eax, 0x00202003
    mov [0x00200000 + 0x300 * 4], eax

    ret

; Input: nothing
; Output: nothing
CreateIdentityPageTable:
    ; Entry index
    mov eax, 0

    ; Entry physical address
    mov ebx, 0

    fill_identity_page_table_loop:
    ; Set flags: read/write, present
    mov ecx, ebx
    or ecx, 3
    
    ; Set entry
    mov [0x00201000 + eax*4], ecx

    ; Go to the next entry
    add ebx, 0x1000
    inc eax

    ; Leave loop if we filled all entries for the first megabyte
    cmp eax, 1024
    jl fill_identity_page_table_loop

    ret

; Input: nothing
; Output: nothing
CreateKernelPageTable:
    ; Entry index
    mov eax, 0

    ; Entry physical address
    mov ebx, 0

    fill_kernel_page_table_loop:
    ; Set flags: read/write, present
    mov ecx, ebx
    or ecx, 3
    
    ; Set entry
    mov [0x00202000 + eax*4], ecx

    ; Go to the next entry
    add ebx, 0x1000
    inc eax

    ; Leave loop if we filled all entries for the first megabyte
    cmp eax, 1024
    jl fill_kernel_page_table_loop

    ret

; Input: nothing
; Output: nothing
EnablePaging:
    ; Set address of the directory table
    mov eax, 0x200000
    mov cr3, eax

    ; Enable paging
    mov ebx, cr0
    or ebx, 0x80000001
    mov cr0, ebx

    ret