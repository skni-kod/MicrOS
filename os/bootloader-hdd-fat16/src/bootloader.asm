;                                            MicrOS physic memory layout
;
; |------------|---------|------------|------------|----------------|---------|------------|------------|---------|---------|------------|------------|------------|------------|
; |   1 KiB    |  256 B  |   21 KiB   |   1 KiB    |     4 KiB      |  3 KiB  |   512 B    |   28 KiB   | 578 KiB |  1 KiB  |  383 KiB   |   15 MiB   |   1 MiB    |   4 MiB    |
; | Interrupts |   BDA   | Floppy DMA | Memory map | Page Directory |  Free   | Bootloader | FAT16 data |  Kernel |  EBDA   | Video, ROM |  Reserved  |   Stack    | Page Table |
; |            |         |            |            |                |         |            |            |         |         |            |            |            |            |
; |  0x00000   | 0x00400 |  0x00500   |  0x05C00   |    0x06000     | 0x07000 |  0x07C00   |  0x07E00   | 0x0F000 | 0x9FC00 |  0xA0000   | 0x00100000 | 0x01000000 | 0x01100000 |
; |  0x003FF   | 0x004FF |  0x05BFF   |  0x05FFF   |    0x06FFF     | 0x07BFF |  0x07DFF   |  0x0EFFF   | 0x9FBDD | 0x9FFFF |  0xFFFFF   | 0x00FFFFFF | 0x010FFFFF | 0x014FFFFF |
; |------------|---------|------------|------------|----------------|---------|------------|------------|---------|---------|------------|------------|------------|------------|
; {                                        Segment 1                                                       }{                            Segment 2 - n                          }

%include "src/memory_layout.asm" ; memory layout struct

[BITS 16]
[ORG FAT.BoostrapCode]

Main:
    ; Disable interrupt and clear direction flag
    cli
    cld
    
    ; Store active partition 
    mov cx, 16
    mov ax, 0x7c0
    mov es, ax
    mov di, MemoryLayout.PTEPlace + PTE.PartitionAttributes
    rep movsb

    ; Set stackpointer under bootloader
    xor ax, ax
    mov ss, ax
    mov esp, 0x7C00
    mov ebp, esp

    ; Initialize data segment register
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax

    ; Store drive number to variable
    mov [MemoryLayout.LocalVariablesPlace + LocalVariables.DriveNumber], dl

    ; Caluclate needed things
    ; FAT Region
    movzx ebx, word [MemoryLayout.BootSector + FAT.ReservedLogicalSectors]
    add ebx, [MemoryLayout.PTEPlace + PTE.PartitionRegionStart]
    mov [MemoryLayout.LocalVariablesPlace + LocalVariables.FatRegionStart], ebx

    ; Root Directory Region
    movzx ax, [MemoryLayout.BootSector + FAT.NumberOfFATs]
    mul word [MemoryLayout.BootSector + FAT.LogicalSectorsPerFAT]
    shr edx, 16
    mov dx, ax
    add ebx, edx
    mov [MemoryLayout.LocalVariablesPlace + LocalVariables.RootDirectoryRegionStart], ebx

    ; DES Entries per Sector
    mov ax, [MemoryLayout.BootSector + FAT.BytesPerLogicalSector]
    mov bh, 32 ;; 32 bytes per DES
    div bh
    mov [MemoryLayout.LocalVariablesPlace + LocalVariables.DESEntriesPerSector], al

    ; Make Check if Int13h extension exisis
    mov dl, [MemoryLayout.LocalVariablesPlace + LocalVariables.DriveNumber]
    call CheckInt13Extenstion

    ; If no exists... fail...
    jc Fail

    ; Search for file name in DES
    ;;cx = 11 - file name length
    ;;dl = drive number
    ;;es:di = pointer to DES file name
    ;;ds:si = pointer to search file name or dap
    ;;bx = index of Sector DES Search

    ;; Initialize DAP
    mov byte [MemoryLayout.DAPPlace + DAP.SizeOfPacket], 0x10
    mov byte [MemoryLayout.DAPPlace + DAP.Reserved], 0x0
    mov word [MemoryLayout.DAPPlace + DAP.NumberOfSectorsToTransfer], 0x1
    mov di, MemoryLayout.DESPlace + DES.Filename
    mov [MemoryLayout.DAPPlace + DAP.TransferBufferOffset], di
    mov [MemoryLayout.DAPPlace + DAP.TransferBufferSegment], es
    mov di, [MemoryLayout.LocalVariablesPlace + LocalVariables.RootDirectoryRegionStart]
    mov [MemoryLayout.DAPPlace + DAP.LowerStartLBA], di
    mov dword [MemoryLayout.DAPPlace + DAP.UpperStartLBA], 0x0

    ;; Initlize Loop Varaibles
    mov bx, [MemoryLayout.BootSector + FAT.NumberOfDirectoryEntries]
    mov [MemoryLayout.LocalVariablesPlace + LocalVariables.DESEntryCounter], bx

FileNameLoop:
    ; Read Sector
    mov si, MemoryLayout.DAPPlace + DAP.SizeOfPacket
    mov ah, 0x42
    int 13h

    xor bx, bx
DESPerSectorSearch:
    mov cx, 11 ; Need to set every loop beacuse cx is affected later 
    mov si, LoaderFileName
    mov di, [MemoryLayout.DESPlace + DES.Filename + bx]
    mov al, [MemoryLayout.DESPlace + DES.Filename + bx] ; write first letter of filename
    call WriteCharacter
    mov al, 0x0d ;CR LF - new line
    call WriteCharacter
    mov al, 0x0a
    call WriteCharacter
    repne cmpsb ; compare file names

    ; File found, jump.
    je FileFound
    
    ; File not found.
    add bx, DES_size

    ; Check if any root directory entries
    dec word [MemoryLayout.LocalVariablesPlace + LocalVariables.DESEntryCounter]
    mov ah, 0x0
    int 16h ;try to wait for keyboard
    jz FileNotFound
    
    ; Check if end of loaded sector
    mov ax, bx
    mov cl, DES_size
    div cl
    cmp al, [MemoryLayout.LocalVariablesPlace + LocalVariables.DESEntriesPerSector]
    
    ; The are more entries to analyze in loaded sector
    jne DESPerSectorSearch

    ; Need to load next sector
    inc dword [MemoryLayout.DAPPlace + DAP.LowerStartLBA]
    jno FileNameLoop

    ;Need to update UpperStartLBA in DAP
    inc dword [MemoryLayout.DAPPlace + DAP.UpperStartLBA]
    jmp FileNameLoop

FileFound:
    mov al, 0x4b
    call WriteCharacter
    hlt

FileNotFound:
    mov al, 0x4e
    call WriteCharacter
    hlt

Fail:
    ; All things go wrong. Snowman doesn't like us.
    mov al, 0x46
    call WriteCharacter
    hlt

WriteCharacter:
    mov ah, 0x0e
    mov bl, 0x07
    int 10h
    ret

%include "src/drivefunctions.asm"

LoaderFileName  db 'KERNEL  BIN'    ; 11 chars