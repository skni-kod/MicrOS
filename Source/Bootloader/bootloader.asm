[BITS 16]
[ORG 0x7C00]

; FAT12 header
; Jump instruction (length: 3)
jmp main
nop

; OEM name (lenght: 8)
db 'MicrOS  '

; Bytes per logical sector (length: 2) = 512
dw 0x0200

; Logical sectors per cluster (length: 1) = 1
db 0x01

; Reserved logical sectors (length: 2) = 1
dw 0x0001

; File allocation tables (length: 1) = 2
db 0x02

; Max root directory entries (length: 2) = 224
dw 0x00E0

; Total logical sectors (length: 2) = 2880
dw 0x0B40

; Media descriptor (length: 1)
; 3.5-inch (90 mm) Double Sided
; 80 tracks per side
; 18 or 36 sectors per track
db 0xF0

; Logical sectors per File Allocation Table (length: 2) = 9
dw 0x0009

; Physical sectors per track (length: 2) = 18
dw 0x0012

; Number of heads (length: 2) = 2
dw 0x0002

; Hidden sectors (length: 4) = 0
dd 0x00000000

; Total logical sectors (length: 4) = 0
dd 0x00000000

; Physical drive number (length: 1)
db 0x00

; INT 13h scratchpad (length: 1)
db 0x00

; Extended boot signature (length: 1)
db 0x29

; Volume ID (length: 8)
dd 0x12345678

; Partition Volume Label (length: 11)
db 'PARTITION 1'

; File system type (length: 8)
db 'FAT12   '

main:
JMP $

times 510 - ($ - $$) db 0
dw 0xAA55