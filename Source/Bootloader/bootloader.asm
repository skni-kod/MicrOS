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

main:
JMP $

times 510 - ($ - $$) db 0
dw 0xAA55