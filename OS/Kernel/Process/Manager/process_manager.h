#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdlib.h>
#include "process_header.h"
#include "../../Memory/Paging/paging.h"
#include "../ELF/Parser/elf_header.h"
#include "../ELF/Loader/elf_loader.h"
#include "kvector.h"
#include "../../FileSystems/FAT/fat.h"
#include "../../Assembly/io.h"
#include "../../Interrupts/IDT/idt.h"

void process_manager_init();
uint32_t process_manager_create_process(char *path);
process_header *process_manager_get_process(uint32_t process_id);

void process_manager_interrupt_handler(interrupt_state *state);

#endif