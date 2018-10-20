[BITS 16]
[ORG 0x7C00]

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
HeadsCount                      dw 0x0002
HiddenSectorsCount              dd 0x00000000
TotalLogicalAndHiddenSectors    dd 0x00000000
PhysicalDriveNumber             db 0x00
INT13Scratchpad                 db 0x00
ExtendedBootSignature           db 0x29
VolumeID                        dd 0x12345678
PartitionVolumeLabel            db 'PARTITION 1'    ; 11 chars
FileSystemType                  db 'FAT12   '       ; 8 chars

; Entry point of bootloader
main:
    ; Set stack pointer to be directly under bootloader
    ; About 30 KiB of memory is free here
    ; https://wiki.osdev.org/Memory_Map_(x86)
    mov esp, 0x7C00
    mov ax, 0
    mov ss, ax

    call reset_floppy

    mov al, 1       ; Number of sectors to read
    mov cl, 2       ; Sector number
    mov bx, 0
    mov es, bx      ; Segment
    mov bx, 0x7E00  ; Offset
    call load_floppy_to_memory

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
load_floppy_to_memory:
    ; Function name: Read Disk Sectors
    mov ah, 0x02

    ; Track number
    mov ch, 0

    ; Head number
    mov dh, 0

    int 0x13
    ret

times 510 - ($ - $$) db 0
dw 0xAA55