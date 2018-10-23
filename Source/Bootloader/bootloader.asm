;                              MicrOS memory layout in real mode
;
; |------------|---------|----------|------------|------------|---------|---------|------------|
; |   1 KiB    |  256 B  |  29 KiB  |   512 B    |   32 KiB   | 574 KiB |  1 KiB  |  383 KiB   |
; | Interrupts |   BDA   |  Stack   | Bootloader | FAT32 data |  Kernel |  EBDA   | Video, ROM |
; |            |         |          |            |            |         |         |            |
; |  0x00000   | 0x00400 |  0x00500 |  0x07C00   |  0x07E00   | 0x10000 | 0x9FC00 |  0xA0000   |
; |  0x003FF   | 0x004FF |  0x07BFF |  0x07DFF   |  0x0FFFF   | 0x9FBDD | 0x9FFFF |  0xFFFFF   |
; |------------|---------|----------|------------|------------|---------|---------|------------|
; {                       Segment 1                           }{        Segment 2 - n          }

[BITS 16]
[ORG 0x7C00]

; Jump to main and align to 3 bytes, we don't want to execute FAT12 header
jmp Main
nop

; FAT header
OEMName                         db 'MicrOS  '       ; 8 chars
BytesPerLogicalSector           dw 0x0200

; Bios parameter block
LogicalSectorsPerCluster        db 0x01
ReservedLogicalSectors          dw 0x0001
FileAllocationTables            db 0x02
MaxRootDirectoryEntries         dw 0x00E0
TotalLogicalSectors             dw 0x0B40
MediaDescriptor                 db 0xF0
LogicalSectorsPerFAT            dw 0x0009
PhysicalSectorsPerTrack         dw 0x0012
Heads                           dw 0x0002
HiddenSectors                   dd 0x00000000
TotalLogicalAndHiddenSectors    dd 0x00000000
PhysicalDriveNumber             db 0x00
INT13Scratchpad                 db 0x00
ExtendedBootSignature           db 0x29
VolumeID                        dd 0x12345678
PartitionVolumeLabel            db 'SYSTEM PART'    ; 11 chars
FileSystemType                  db 'FAT12   '       ; 8 chars

KernelFileName                  db 'KERNEL  BIN'    ; 11 chars
NonDataSectors                  db 0x00

BootloaderInitString            db 'MicrOS Bootloader', 0x0D, 0x0A, 0

; Entry point of bootloader
Main:
    ; Set stack pointer to be directly under bootloader
    ; About 30 KiB of memory is free here
    ; https://wiki.osdev.org/Memory_Map_(x86)
    mov esp, 0x7C00
    mov ebp, esp

    ; Display bootloader startup message
    mov si, BootloaderInitString
    call PrintString

    ; Save device number (DL register) to memory
    mov [INT13Scratchpad], dl

    ; Save non data sectors count to memory
    call GetNonDataSectorsCount
    mov [NonDataSectors], cx

    JMP $

; Input: nothing
; Output:
;   - ah - disk operation status (http://stanislavs.org/helppc/int_13-1.html)
;   - cf - 1 if successful, 0 if error
ResetFloppy:
    ; Function name: Reset Disk System
    mov ah, 0

    int 0x13

    ret

; Input:
;   - al - sectors count
;   - cl - sector number
;   - es - segment
;   - bx - offset
; Output:
;   - ah - status (http://stanislavs.org/helppc/int_13-1.html)
;   - al - number of sectors read
;   - cf - 1 if successful, 0 if error
LoadFloppyData:
    ; Function name: Read Disk Sectors
    mov ah, 0x02

    ; Track number
    mov ch, 0

    ; Head number
    mov dh, 0

    ; Drive number
    mov dl, [INT13Scratchpad]

    int 0x13

    ret

; Input: nothing
; Output:
;   cx - number of floppy sectors with metadata
GetNonDataSectorsCount:

    ; Boot sector
    mov cx, 1

    ; FAT sectors (number of FATs * sectors per FAT)
    mov al, [FileAllocationTables]
    mov dx, [LogicalSectorsPerFAT]
    mul dx
    add cx, ax

    ; Root directory sectors (number of root entries * 32 / bytes per sector)
    mov ax, [MaxRootDirectoryEntries]
    mov dx, 32
    mul dx
    xor dx, dx
    mov bx, [BytesPerLogicalSector]
    div bx
    add cx, ax

    ret

; Input: nothing
;   si - address of the string ended with null char
; Output: nothing
PrintString:
    lodsb
    
    or al, al
    jz PrintString_End
    
    ; Function name: Teletype Output
    mov ah, 0x0E
    int 0x10

    ; Page number and color to 0
    mov bx, 0
    
    jmp PrintString

    PrintString_End:
    ret

times 510 - ($ - $$) db 0
dw 0xAA55