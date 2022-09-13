#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include "process/elf/parser/elf_parser.h"
#include "cpu/paging/paging.h"
#include "memory/physical/physical_memory_manager.h"

uint32_t elf_loader_load(uint8_t *content);

#endif