#ifndef GDT_H
#define GDT_H

#define GDT_SIZE 6

#include <stdbool.h>
#include "gdt_entry.h"
#include "gdt_info.h"
#include "cpu/tss/tss_entry.h"

void gdt_init();
void gdt_fill_entry(int index, bool executable, uint8_t privilege_level);
void gdt_fill_entry_as_tss(int index, tss_entry *tss);

#endif