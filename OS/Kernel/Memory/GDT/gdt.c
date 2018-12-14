#include "gdt.h"

gdt_entry gdt[GDT_SIZE];
gdt_info info;

void gdt_init()
{
    gdt_fill_entry(1, true, 0);
    gdt_fill_entry(2, false, 0);
    gdt_fill_entry(3, true, 3);
    gdt_fill_entry(4, false, 3);

    info.size = sizeof(gdt_entry) * GDT_SIZE - 1;
    info.offset = (uint32_t)&gdt;

    __asm__("lgdt %0" ::"m"(info));
}

void gdt_fill_entry(int index, bool executable, uint8_t privilege_level)
{
    gdt[index].limit_0_15 = 0xFFFF;
    gdt[index].base_0_15 = 0x0000;
    gdt[index].base_16_23 = 0x00;

    gdt[index].accessed = 0;
    gdt[index].read_write = 1;
    gdt[index].direction = 0;
    gdt[index].executable = executable;
    gdt[index].reserved_1 = 1;
    gdt[index].privilege_level = privilege_level;
    gdt[index].present = 1;

    gdt[index].limit_16_19 = 0xf;
    gdt[index].reserved_2 = 0;
    gdt[index].size = 1;
    gdt[index].granularity = 1;

    gdt[index].base_24_31 = 0x00;
}