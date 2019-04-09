#ifndef ELF_HEADER_H
#define ELF_HEADER_H

#include <stdint.h>

typedef enum elf_instruction_set
{
    elf_instruction_set_32_bit = 1,
    elf_instruction_set_64_bit = 2
} elf_instruction_set;

typedef enum elf_endian
{
    little_endian = 1,
    big_endian = 2
} elf_endian;

typedef enum elf_type
{
    elf_type_relocatable = 1,
    elf_type_executable = 2,
    elf_type_shared = 3,
    elf_type_core = 4,

    _elf_type_extend_to_16_bit = 0xFFFF
} elf_type;

typedef enum elf_architecture
{
    elf_architecture_unspecified = 0,
    elf_architecture_sparc = 2,
    elf_architecture_x86 = 3,
    elf_architecture_mips = 8,
    elf_architecture_power_pc = 0x14,
    elf_architecture_arm = 0x28,
    elf_architecture_super_h = 0x2A,
    elf_architecture_ia_64 = 0x32,
    elf_architecture_x86_64 = 0x3E,
    elf_architecture_aarch64 = 0xB7,

    _elf_architecture_extend_to_16_bit = 0xFFFF
} elf_architecture;

typedef struct elf_header
{
    uint8_t magic_number;
    uint8_t elf_ascii[3];
    elf_instruction_set instruction_set;
    elf_endian endian;
    uint8_t version_8;
    uint8_t os_abi;
    uint64_t reserved;
    elf_type type;
    elf_architecture architecture;
    uint32_t version_32;
    uint32_t entry_position;
    uint32_t program_header_table_position;
    uint32_t section_header_table_position;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_header_table_entry_size;
    uint16_t program_header_table_entries_count;
    uint16_t section_header_table_entry_in_section_size;
    uint16_t section_header_table_entries_in_section_count;
    uint16_t section_header_table_section_names_index;
} __attribute__((packed)) elf_header;

#endif