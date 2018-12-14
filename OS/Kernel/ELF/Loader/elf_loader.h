#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include "../Parser/elf_parser.h"
#include "../../Memory/Paging/paging.h"
#include "../../Memory/Manager/Physic/physical_memory_manager.h"

void elf_loader_load(uint8_t *content);

#endif