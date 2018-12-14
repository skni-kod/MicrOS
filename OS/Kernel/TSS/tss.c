#include "tss.h"

tss_entry tss;

void tss_init()
{
    gdt_fill_entry_as_tss(5, &tss);
    tss_fill_entry(0xC1100000, 0x10);

    __asm__("mov $0x28, %%ax\n"
            "ltr %%ax" ::);
}

void tss_fill_entry(uint32_t esp0, uint32_t ss0)
{
    tss.previous_tss = 0;
    tss.esp0 = esp0;
    tss.ss0 = ss0;
    tss.esp1 = 0;
    tss.ss1 = 0;
    tss.esp2 = 0;
    tss.ss2 = 0;
    tss.cr3 = 0;
    tss.eip = 0;
    tss.eflags = 0;
    tss.eax = 0;
    tss.ecx = 0;
    tss.edx = 0;
    tss.ebx = 0;
    tss.esp = 0;
    tss.ebp = 0;
    tss.esi = 0;
    tss.edi = 0;
    tss.es = 0;
    tss.cs = 0;
    tss.ss = 0;
    tss.ds = 0;
    tss.fs = 0;
    tss.gs = 0;
    tss.ldt = 0;
    tss.trap = 0;
    tss.iomap_base = 0;
}