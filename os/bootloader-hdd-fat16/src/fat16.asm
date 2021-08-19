struc FAT
    .DummyJump resb 3 ;0x0
    .OEMName resb 8 ; 0x3

    ; Standard BPB
    .BytesPerLogicalSector resw 1 ;0xb
    .LogicalSectorsPerCluster resb 1 ;x0d
    .ReservedLogicalSectors resw 1 ;x0e
    .NumberOfFATs resb 1 ;x10
    .NumberOfDirectoryEntries resw 1 ;x11
    .TotalLogicalSectors resw 1 ;x13
    .MediaDescriptor resb 1 ;x15
    .LogicalSectorsPerFAT resw 1 ;x16
    .PhysicalSectorsPerTrack resw 1 ;x18
    .HeadsForDisk resw 1 ;x1a
    .HiddenSectorsCount resd 1 ;x1c
    .LargeTotalLogicalSectors resd 1 ;x20

    ; Extended BPB (DOS 4.0)
    .PhysicalDriveNumber resb 1 ;x24
    .NTFlags resb 1 ;x25
    .ExtendedBootSignature resb 1 ;x26
    .VolumeID resd 1 ;x27
    .PartitionVolumeLabel resb 11 ;x2b
    .FileSystemType resb 8 ;x3c ; Don't use to identify file system. It's only string.

    ; Bootstrap code and 0x55AA BootSignature
    .BoostrapCode resb 448
    .BootSignature resb 2
endstruc