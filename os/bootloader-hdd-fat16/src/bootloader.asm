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

BYTES_PER_SECTOR equ 512; assuming that disk has 512 bytes per sector
NUMBER_OF_DES_ENTRIES equ 512; assuming that in FAT 16 Root Directory can contains 512 entries
DES_ENTRIES_PER_SECTOR equ 32; assuming that we load 32 DES entries in one sector

; KNOW PLACES OF NEEDED VARIABLES:
;[ebp - 4] - place of PartitionRegionStart
;[ebp - 6] - place of drive number (byte)
;[ebp - 10] - place of Root Directory Region (dword)
;[ebp - 12] - DESEntryCounter (word)


Main:
    ; Disable interrupt and clear direction flag
    cli
    cld

    ; Store active partition 
    ;mov cx, PTE_size
    ;mov ax, 0x7c0
    ;mov es, ax
    ;mov di, MemoryLayout.PTEPlace + PTE.PartitionAttributes
    ;rep movsb

    ; Set stackpointer under bootloader
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00
    mov bp, sp

    ; Store active partition region start
    push dword [si + PTE.PartitionRegionStart]

    ; Initialize data segment register
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax

    ; Store drive number to variable
    push dx ;;[ebp - 6] - place of drive number (byte)

    ; Caluclate needed things
    ; FAT Region
    ;movzx ebx, word [MemoryLayout.BootSector + FAT.ReservedLogicalSectors]
    ;add ebx, dword [bp - 4]

    ; Root Directory Region
    ;movzx eax, word [MemoryLayout.BootSector + FAT.LogicalSectorsPerFAT]
    ;movzx edx, byte [MemoryLayout.BootSector + FAT.NumberOfFATs]
    ;imul eax, edx
    ;add ebx, edx
    ;push ebx ;;[ebp - 10] - place of Root Directory Region (word)
    ;movzx ax, [MemoryLayout.BootSector + FAT.NumberOfFATs]
    ;mul word [MemoryLayout.BootSector + FAT.LogicalSectorsPerFAT]
    ;shl edx, 16
    ;mov dx, ax
    ;add ebx, edx
    ;push ebx ;;[ebp - 10] - place of Root Directory Region (word)


    ;call CheckInt13Extenstion
    mov ah, 0x41
    mov bx, 0xaa55

    ; Call interrupt
    int 0x13

    ; If no exists... fail...
    jc Fail


    movzx eax, byte [bp + FAT.NumberOfFATs]
    mul word [bp + FAT.LogicalSectorsPerFAT]
    add ax, [bp + FAT.ReservedLogicalSectors]
    add eax, dword [bp - 4]
    push eax ;;[ebp - 10] - place of Root Directory Region (word)

    ; Search for file name in DES
    ;;cx = 11 - file name length
    ;;dl = drive number
    ;;ds:si = pointer to DES file name, or dap
    ;;es:di = pointer to search file name
    ;;bx = index of Sector DES Search

    ;; Initialize DAP
    ;mov byte [MemoryLayout.DAPPlace + DAP.SizeOfPacket], 0x10
    ;mov byte [MemoryLayout.DAPPlace + DAP.Reserved], 0x0
    ;mov word [MemoryLayout.DAPPlace + DAP.NumberOfSectorsToTransfer], 0x1
    mov si, MemoryLayout.DAPPlace + DAP.SizeOfPacket
    mov dword [si + DAP.SizeOfPacket], 0x00010010
    ;mov word [MemoryLayout.DAPPlace + DAP.TransferBufferOffset], MemoryLayout.DESPlace + DES.Filename
    ;mov [MemoryLayout.DAPPlace + DAP.TransferBufferSegment], es
    ;assuming that es was not changed until here, and contains value = 0x7c0
    mov dword [si + DAP.TransferBufferOffset], 0x07c00220; this magic number es concatanete with MemoryLayout.DESPlace + DES.Filename 
    ;mov di, [bp - 10]
    mov [si + DAP.LowerStartLBA], eax
    mov dword [si + DAP.UpperStartLBA], 0x0

    ;; Initlize Loop Varaibles
    push word NUMBER_OF_DES_ENTRIES ;;[ebp - 12] - DESEntryCounter (word)
    ;;mov bx, [MemoryLayout.BootSector + FAT.NumberOfDirectoryEntries]
    ;;mov [MemoryLayout.LocalVariablesPlace + LocalVariables.DESEntryCounter], bx

FileNameLoop:
    ; Read Sector
    ;mov si, MemoryLayout.DAPPlace + DAP.SizeOfPacket
    mov ah, 0x42
    mov dl, [bp - 6]
    int 13h

    xor bx, bx
DESPerSectorSearch:
    mov cx, 11 ; Need to set every loop beacuse cx is affected later 
    mov di, LoaderFileName
    push si
    mov si, MemoryLayout.DESPlace + DES.Filename
    add si, bx

    mov ax, [si + DES.Attribute] ; checking attributes
    and ax, 0x58
    jnz WrongEntry ; The entry is label, directory, device
    not ax
    test ax, 0xf
    jz WrongEntry ; The entry is vfat long name
    repe cmpsb ; compare file names

    ; File found, jump.
    je FileFound
    
WrongEntry:
    ; File not found.
    add bx, DES_size
    pop si
    ; Check if any root directory entries
    dec word [bp - 12]
    jz FileNotFound
    
    ; Check if end of loaded sector
    mov ax, bx
    mov cl, DES_size
    div cl
    cmp al, DES_ENTRIES_PER_SECTOR
    
    ; The are more entries to analyze in loaded sector
    jne DESPerSectorSearch

    ; Need to load next sector
    inc dword [MemoryLayout.DAPPlace + DAP.LowerStartLBA]
    jno FileNameLoop

    ;Need to update UpperStartLBA in DAP
    inc dword [MemoryLayout.DAPPlace + DAP.UpperStartLBA]
    jmp FileNameLoop

FileFound:
    ; Save loader structure for feature use
    sub si, 11 ; go back to begin of file name
    mov di, MemoryLayout.DESPlace + DES.Filename
    mov cx, DES_size
    rep movsb

    ;Loading clusters
    ;;dl = drive number
    ;;ds:si = DAP
    ;;ah = 0x42

    ;Calculate sector where start
    ;; Calculate Data Region Start
    ;mov eax, (NUMBER_OF_DES_ENTRIES * 32) / BYTES_PER_SECTOR
    ;add eax, dword [bp - 10]
    ;push eax

    ;; Translate start cluster to start sector
    ;xor edx, edx
    ;mov ax, word [MemoryLayout.DESPlace + DES.StartCluster]
    ;sub ax, 2
    ;movzx bx, byte [MemoryLayout.BootSector + FAT.LogicalSectorsPerCluster]
    ;mul bx
    ;shl edx, 16
    ;mov dx, ax

    ;; Add Data Region Start to Start Sector
    ;pop eax
    ;add edx, eax
    ;push edx ;;start sector

    mov ecx, (NUMBER_OF_DES_ENTRIES * 32) / BYTES_PER_SECTOR
    add ecx, dword [bp - 10]

    movzx ebx, word [MemoryLayout.DESPlace + DES.StartCluster]
    sub ebx, 2

    movzx edx, byte [MemoryLayout.BootSector + FAT.LogicalSectorsPerCluster]
    ;movzx eax, ax
    imul ebx, edx
    add ebx, ecx

    ;Calculate how many sectors to copy
    ;mov eax, [MemoryLayout.DESPlace + DES.FileSize]
    ;mov edx, eax
    ;shr edx, 16
    ;mov bx, BYTES_PER_SECTOR
    ;div bx
    ;cmp dx, 0
    ;je NoNeedToRoundUp
    ;inc ax ;;need to round up
    mov edx, [MemoryLayout.DESPlace + DES.FileSize]
    mov eax, edx
    and dx, 511
    shr eax, 9
    cmp dx, 1
    sbb ax, -1
    
NoNeedToRoundUp:
    ;;in eax we have number of sectors to read
    movzx ecx, ax
    mov si, MemoryLayout.DAPPlace + DAP.SizeOfPacket
    mov [si + DAP.LowerStartLBA], ebx
    mov dword [si + DAP.UpperStartLBA], 0
    mov dword [si + DAP.TransferBufferOffset], 0xf000
    ;; assume that non segment registers are changed
    mov word [si + DAP.NumberOfSectorsToTransfer], 1
ReadSector:
    mov dl, [bp - 6]
    mov ah, 0x42
    int 13h
    ;mov ax, word [MemoryLayout.DAPPlace + DAP.TransferBufferOffset]
    add word [si + DAP.TransferBufferOffset], BYTES_PER_SECTOR
    jnc NotSegmentOverflow
    add word [si + DAP.TransferBufferSegment], 0x1000
NotSegmentOverflow:
    ;mov [MemoryLayout.DAPPlace + DAP.TransferBufferOffset], ax
    inc word [si + DAP.LowerStartLBA]
    loop ReadSector

    jmp 0x0000:0xF000 ;;JUMP TO KERNEL

FileNotFound:
    mov al, 0x0e4e
    jmp HaltOnError
    ;call WriteCharacter
    ;hlt

Fail:
    ; All things go wrong. Snowman doesn't like us.
    mov ax, 0x0e46
    jmp HaltOnError
    ;call WriteCharacter
    ;hlt

HaltOnError:
    mov bl, 0x07
    int 10h
    hlt

;WriteCharacter:
;    mov ah, 0x0e
;    mov bl, 0x07
;    int 10h
;    ret

;%include "src/drivefunctions.asm"

LoaderFileName  db 'KERNEL  BIN'    ; 11 chars
