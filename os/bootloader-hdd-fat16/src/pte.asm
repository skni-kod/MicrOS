struc PTE
    .PartitionAttributes resb 1
    .CHSStart resb 3
    .PartitionType resb 1
    .CHSStop resb 3
    .PartitionRegionStart resd 4
    .PartitionLength resd 4
endstruc