#ifndef ELF_PROGRAM_HEADER_H
#define ELF_PROGRAM_HEADER_H

#include <stdint.h>

typedef enum elf_type_of_segment
{
    elf_type_of_segment_load = 1,
    elf_type_of_segment_dynamic = 2,
    elf_type_of_segment_iterpreter = 3,
    elf_type_of_segment_note = 4,

    _elf_type_of_segment_extend_to_32_bit = 0xFFFFFFFF
} elf_type_of_segment;

typedef struct elf_program_header_flags
{
    uint8_t executable : 1;
    uint8_t writable : 1;
    uint8_t readable : 1;
    uint8_t reserved : 5;
} __attribute__((packed)) elf_program_header_flags;

typedef struct elf_program_header
{
    elf_type_of_segment type_of_segment;
    uint32_t offset;
    uint32_t virtual_address;
    uint32_t reserved;
    uint32_t size_in_file;
    uint32_t size_in_memory;
    elf_program_header_flags flags;
    uint32_t alignment;
} __attribute__((packed)) elf_program_header;

#endif