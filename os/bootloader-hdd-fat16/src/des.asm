struc DES
    .Filename resb 11
    .Attribute resb 1
    .NTReserverd resb 1
    .CreationStamp resb 1
    .CreationTime resw 1
    .CreationDate resw 1
    .LastAccessDate resw 1
    .FAT32Reserved resw 1
    .LastWriteTime resw 1
    .LastWriteDate resw 1
    .StartCluster resw 1
    .FileSize resd 1
endstruc