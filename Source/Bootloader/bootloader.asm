;                                    MicrOS memory layout
;
; |------------|---------|----------|------------|------------|---------|---------|------------|
; |   1 KiB    |  256 B  |  29 KiB  |   512 B    |   32 KiB   | 574 KiB |  1 KiB  |  383 KiB   |
; | Interrupts |   BDA   |  Stack   | Bootloader | FAT12 data |  Kernel |  EBDA   | Video, ROM |
; |            |         |          |            |            |         |         |            |
; |  0x00000   | 0x00400 |  0x00500 |  0x07C00   |  0x07E00   | 0x10000 | 0x9FC00 |  0xA0000   |
; |  0x003FF   | 0x004FF |  0x07BFF |  0x07DFF   |  0x0FFFF   | 0x9FBDD | 0x9FFFF |  0xFFFFF   |
; |------------|---------|----------|------------|------------|---------|---------|------------|
; {                       Segment 1                           }{        Segment 2 - n          }

[BITS 16]
[ORG 0x7C00]

; Jump to main and align to 3 bytes, we don't want to execute FAT12 header
jmp main
nop

OEMName                         db 'MicrOS  '       ; 8 chars
BytesPerLogicalSector           dw 0x0200
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
PartitionVolumeLabel            db 'PARTITION 1'    ; 11 chars
FileSystemType                  db 'FAT12   '       ; 8 chars

KernelFileName                  db 'KERNEL  BIN'    ; 11 chars
ReservedSectors                 db 0x00

; Entry point of bootloader
main:
    ; Set stack pointer to be directly under bootloader
    ; About 30 KiB of memory is free here
    ; https://wiki.osdev.org/Memory_Map_(x86)
    mov esp, 0x7C00
    mov ebp, esp
    mov ax, 0
    mov ss, ax

    ; Save device number (DL register) to memory
    mov [INT13Scratchpad], dl

    ; Save reserved sectors count to memory
    call calculate_reserved_fat_area
    mov [ReservedSectors], cx

    call reset_floppy

    ; Load FAT12 tables and root directory to memory
    mov al, [ReservedSectors]       ; Number of sectors to read
    sub al, 1

    mov cl, 2                       ; Sector number
    mov bx, 0
    mov es, bx                      ; Segment
    mov bx, 0x7E00                  ; Offset
    call load_floppy_data

    call find_kernel_first_sector
    call load_kernel

    JMP $

; Input: nothing
; Output:
;   - ah - disk operation status (http://stanislavs.org/helppc/int_13-1.html)
;   - cf - 1 if successful, 0 if error
reset_floppy:
    ; Function name: Reset Disk System
    mov ah, 0

    int 0x13
    ret

; Input:
;   - al - number of sectors to read
;   - cl - sector number
;   - es - segment
;   - bx - offset
; Output:
;   - ah - status (http://stanislavs.org/helppc/int_13-1.html)
;   - al - number of sectors read
;   - cf - 1 if successful, 0 if error
load_floppy_data:
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
calculate_reserved_fat_area:
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
; Output:
;   ax - first kernel sector number
find_kernel_first_sector:
    ; Set initial offset of floppy data
    mov ecx, 0x7E00

    ; Add size of both File Allocations Tables to get root directory offset
    mov al, [FileAllocationTables]
    mov dx, [LogicalSectorsPerFAT]
    mul dx

    mov dx, [BytesPerLogicalSector]
    mul dx

    ; Now cx contains offset of root directory
    add cx, ax

    begin_compare_loop:
    ; Load name of entry and expected kernel name offsets
    mov si, cx
    mov di, KernelFileName

    ; Current string index will be stored in dx register
    mov dx, 0

    compare_loop:
    ; Check if whole string has been compared
    cmp dx, 10
    je kernel_found

    ; Compare chars from both strings
    mov ax, [si]
    mov bx, [di]
    cmp ax, bx

    ; If chars aren't equal, go to next entry
    jne filename_not_equal

    ; Increment index and char addresses
    inc dx
    inc si
    inc di

    ; Go to next iteration
    jmp compare_loop

    filename_not_equal:
    ; Go to next entry
    add cx, 0x20
    jmp begin_compare_loop

    kernel_found:
    ; Move address with sector number to bx register due to Intel limitations and then move sector number to ax register
    ; https://stackoverflow.com/questions/1797765/assembly-invalid-effective-address
    mov bx, cx
    add bx, 0x1A
    mov ax, [bx]

    ret

; Input:
;   ax - first kernel sector number
; Output: nothing
load_kernel:
    ; Add kernel sector to base sector offset
    mov cx, [ReservedSectors]
    add cx, ax
    sub cx, 1

    mov al, 1       ; Number of sectors to read
    mov bx, 0x1000
    mov es, bx      ; Segment
    mov bx, 0x0000  ; Offset
    call load_floppy_data

    ; Set initial FAT offset
    mov bx, 0x7E00

    ; Store first sector number in ax register
    ; mov ah, [bx + 1]
    ; mov al, [bx]
    ; and ax, 0x0FFF

    ; Store second sector number in cx register
    ; mov ch, [bx + 2]
    ; mov cl, [bx + 1]
    ; shr cx, 4

    ; TODO: Read rest of the kernel sectors

    ret

times 510 - ($ - $$) db 0
dw 0xAA55