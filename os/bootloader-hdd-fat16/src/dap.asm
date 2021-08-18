; STRUCTURE DAP (Disk Address Packet)
struc DAP
    .SizeOfPacket:              resb 1
    .Reserved:                  resb 1
    .NumberOfSectorsToTransfer: resw 1
    .TransferBufferOffset:      resw 1
    .TransferBufferSegment:      resw 1
    .LowerStartLBA:             resd 1
    .UpperStartLBA:             resd 1
endstruc