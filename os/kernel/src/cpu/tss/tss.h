#ifndef TSS_H
#define TSS_H

#include "tss_entry.h"
#include "cpu/gdt/gdt.h"

void tss_init();
void tss_fill_entry(uint32_t esp0, uint32_t ss0);

#endif