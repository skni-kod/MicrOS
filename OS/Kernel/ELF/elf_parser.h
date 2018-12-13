#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include "elf_header.h"

elf_header *elf_parse(uint8_t *content);

#endif