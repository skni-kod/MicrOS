#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include "elf_header.h"
#include "elf_program_header.h"
#include "elf_section_header.h"

elf_header *elf_get_header(uint8_t *content);
elf_program_header *elf_get_program_header(uint8_t *content);
elf_section_header *elf_get_section_header_with_index(uint8_t *content, uint8_t index);
uint32_t elf_get_total_size_in_memory(uint8_t *content);

#endif