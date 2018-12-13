#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include "elf_header.h"
#include "elf_section_header.h"

elf_header *elf_parse(uint8_t *content);
elf_section_header *elf_parse_text_header(uint8_t *content);
elf_section_header *elf_parse_data_header(uint8_t *content);

#endif