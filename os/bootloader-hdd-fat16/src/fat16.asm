struc FAT
    .DummyJump resb 3
    .OEMName resb 8

    ; Standard BPB
    .BytesPerLogicalSector resw 1
    .LogicalSectorsPerCluster resb 1
    .ReservedLogicalSectors resw 1
    .NumberOfFATs resb 1
    .NumberOfDirectoryEntries resw 1
    .TotalLogicalSectors resw 1
    .MediaDescriptor resb 1
    .LogicalSectorsPerFAT resw 1
    .PhysicalSectorsPerTrack resw 1
    .HeadsForDisk resw 1
    .HiddenSectorsCount resd 1
    .LargeTotalLogicalSectors resd 1

    ; Extended BPB (DOS 4.0)
    .PhysicalDriveNumber resb 1
    .NTFlags resb 1
    .ExtendedBootSignature resb 1
    .VolumeID resd 1
    .PartitionVolumeLabel resb 11
    .FileSystemType resb 8 ; Don't use to identify file system. It's only string.

    ; Bootstrap code and 0x55AA BootSignature
    .BoostrapCode resb 448
    .BootSignature resb 2
endstruc