#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include "elf_header.h"
#include "elf_program_header.h"

elf_header *elf_get_header(uint8_t *content);
elf_program_header *elf_get_program_header(uint8_t *content);

#endif