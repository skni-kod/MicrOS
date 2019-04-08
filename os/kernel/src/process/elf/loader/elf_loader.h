#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include "Process/ELF/Parser/elf_parser.h"
#include "Memory/Paging/paging.h"
#include "Memory/Manager/Physic/physical_memory_manager.h"

uint32_t elf_loader_load(uint8_t *content);

#endif