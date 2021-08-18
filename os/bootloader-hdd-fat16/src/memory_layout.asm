%include "src/fat16.asm" ; 512 Bytes of FAT Bootsector
%include "src/pte.asm" ; 16 Bytes of Active Partition Entry
%include "src/dap.asm" ; 16 Bytes of DAP structure

struc LocalVariables
    .DriveNumber resb 1
    .FatRegionStart resd 1
    .RootDirectoryRegionStart resd 1
    .DESEntryCounter resw 1
    .DESEntriesPerSector resb 1
endstruc

%include "src/des.asm" ; 32 Bytes of DES structure

struc MemoryLayout
    .BootSector resb FAT_size
    .PTEPlace resb PTE_size
    .DAPPlace resb DAP_size
    .LocalVariablesPlace resb LocalVariables_size
    .DESPlace resb DES_size
    .FATTableSlice resb 4096
endstruc