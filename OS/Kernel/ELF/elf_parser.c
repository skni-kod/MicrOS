#include "elf_parser.h"

elf_header *elf_get_header(uint8_t *content)
{
    return (elf_header *)content;
}

elf_program_header *elf_get_program_header(uint8_t *content)
{
    elf_header *header = elf_get_header(content);
    return (elf_program_header *)(content + header->program_header_table_position);
}

elf_section_header *elf_get_section_header_with_index(uint8_t *content, uint8_t index)
{
    elf_header *header = elf_get_header(content);
    return (elf_section_header *)(content + header->section_header_table_position + (index * sizeof(elf_section_header)));
}