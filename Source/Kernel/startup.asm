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

[BITS 32]

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