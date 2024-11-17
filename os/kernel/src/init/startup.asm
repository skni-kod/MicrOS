;                                   MicrOS virtual memory layout
; |---------------|------------|------------|-------------|------------|-------------------------|
; |     1 MiB     |   15 MiB   |   1 Mib    |    4 MiB    |   3 MiB    |        1017 MiB         |
; | BIOS & Kernel |  Reserved  |   Stack    | Page tables |  Reserved  | Dynamic allocation area |
; |               |            |            |             |            |                         | 
; |  0xC0000000   | 0xC0100000 | 0xC1000000 | 0xC1100000  | 0xC1500000 |       0xC1800000        |
; |  0xC00FFFFF   | 0xC0FFFFFF | 0xC10FFFFF | 0xC14FFFFF  | 0xC17FFFFF |       0xFFFFFFFF        |
; |---------------|------------|------------|-------------|--------------------------------------|
; {     Page 0       }{1}{2}{3}{        Page 4       }{     Page 5     }{       Page 6 - n       }

[BITS 32]

%define PAGE_DIRECTORY_BASE 0x00006000
%define PAGE_TABLES_BASE    0x01100000
%define PAGES_COUNT         6

; %define PAGE_DIRECTORY_BASE 0x09000000
; %define PAGE_TABLES_BASE    0x0A000000
; %define PAGES_COUNT         6

jmp main_protected_area

main_protected_area:
    cli
    ; Set data and stack segments to the third GDI descriptor
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call clear_page_directory
    ;call protstr
    ;cli
    ;hlt
    
    call clear_page_tables

    ;call protstr
    ;cli
    ;hlt

    call create_page_directory
    call create_identity_page_table
    call create_kernel_page_table
    call enable_paging  
    
    ; Set new stack with virtual address
    mov eax, 0xC1100000
    mov esp, eax



    ; Init FPU
    finit
    
    ; call protstr
    ; cli
    ; hlt

    ; Jump to kmain and start kernel work
    extern kmain
    call kmain

    ; Something went wrong, but no problem!
    JMP $
    
; Output: nothing
clear_page_directory:
    ; Index
    mov eax, 0

    clear_page_directory_loop:
    ; Clear entry
    mov [PAGE_DIRECTORY_BASE + eax], byte 0

    ; Increment index
    inc eax

    ; Leave loop if we cleared all entries
    cmp eax, 0x1000
    jl clear_page_directory_loop

    ret

; Input: nothing
; Output: nothing
clear_page_tables:
    ; Index
    mov eax, 0

    clear_page_tables_loop:
    ; Clear entry
    mov [PAGE_TABLES_BASE + eax], byte 0

    ; Increment index
    inc eax

    ; Leave loop if we cleared all entries
    cmp eax, 0x400000
    jl clear_page_tables_loop

    ;;call protstr
    ;;cli
    ;;hlt

    ret

; Input: nothing
; Output: nothing
create_page_directory:
    ; Page definition macro
    %macro page_directory_definition 3
    mov eax, %1 + 3
    mov [%2 + %3], eax
    %endmacro
    
    ; Add temporary identity entry (physical address: 0x00000000, virtual address: 0x00000000, 24 MB)
    %assign i 0 
    %rep    PAGES_COUNT 
    page_directory_definition PAGE_TABLES_BASE + (i * 0x1000), PAGE_DIRECTORY_BASE, i * 4
    %assign i i+1 
    %endrep

    ; Add kernel entry (physical address: 0x00000000, virtual address: 0xC0000000, 24 MB)
    %assign i 0 
    %rep    PAGES_COUNT 
    page_directory_definition PAGE_TABLES_BASE + 0x00300000 + (i * 0x1000), PAGE_DIRECTORY_BASE + 0xC00, i * 4
    %assign i i+1 
    %endrep

    ret

; Input: nothing
; Output: nothing
create_identity_page_table:
    ; Entry index
    mov eax, 0

    ; Entry physical address
    mov ebx, 0

    fill_identity_page_table_loop:
    ; Set flags: read/write, present
    mov ecx, ebx
    or ecx, 3
    
    ; Set entry
    mov [PAGE_TABLES_BASE + eax*4], ecx

    ; Go to the next entry
    add ebx, 0x1000
    inc eax

    ; Leave loop if we filled all entries for the first 6 megabytes
    cmp eax, PAGES_COUNT * 0x400
    jl fill_identity_page_table_loop

    ret

; Input: nothing
; Output: nothing
create_kernel_page_table:
    ; Entry index
    mov eax, 0

    ; Entry physical address
    mov ebx, 0

    fill_kernel_page_table_loop:
    ; Set flags: read/write, present
    mov ecx, ebx
    or ecx, 3
    
    ; Set entry
    mov [PAGE_TABLES_BASE + 0x00300000 + eax*4], ecx

    ; Go to the next entry
    add ebx, 0x1000
    inc eax

    ; Leave loop if we filled all entries for the first megabyte
    cmp eax, PAGES_COUNT * 0x400
    jl fill_kernel_page_table_loop

    ret

; Input: nothing
; Output: nothing
enable_paging:
    ; Set address of the directory table
    mov eax, PAGE_DIRECTORY_BASE
    mov cr3, eax

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000020
    mov cr0, eax

    jmp .branch
    nop
    nop
    nop
    nop
    nop
    .branch:

    ret

; Function for displaying debug text in protected mode
protstr:
    
    mov edi, 0xb8000
    mov ax, 0x0941
    mov ecx, 10
    rep stosw

    ret
