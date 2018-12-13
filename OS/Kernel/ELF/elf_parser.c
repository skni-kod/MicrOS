#include "elf_parser.h"

elf_header *elf_parse(uint8_t *content)
{
    return (elf_header *)content;
}

elf_section_header *elf_parse_text_header(uint8_t *content)
{
    elf_header *header = elf_parse(content);
    return (elf_section_header *)(content + header->program_header_table_position);
}

elf_section_header *elf_parse_data_header(uint8_t *content)
{
    elf_header *header = elf_parse(content);
    return (elf_section_header *)(content + header->section_header_table_position);
}