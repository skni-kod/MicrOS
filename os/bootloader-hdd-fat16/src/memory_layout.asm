%include "src/fat16.asm" ; 512 Bytes of FAT Bootsector
%include "src/pte.asm" ; 16 Bytes of Active Partition Entry
%include "src/dap.asm" ; 16 Bytes of DAP structure
%include "src/des.asm" ; 32 Bytes of DES structure

struc MemoryLayout
    .BootSector resb FAT_size
    .PTEPlace resb PTE_size
    .DAPPlace resb DAP_size
    .DESPlace resb DES_size
endstruc