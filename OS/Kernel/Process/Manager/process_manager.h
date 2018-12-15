#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <Custom/vector.h>
#include <stdlib.h>
#include "process_header.h"
#include "../../Memory/Paging/paging.h"
#include "../ELF/Parser/elf_header.h"

void process_manager_init();
void process_manager_start_process(char *path);

#endif