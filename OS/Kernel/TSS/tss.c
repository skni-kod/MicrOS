#include "tss.h"

tss_entry tss[TSS_SIZE];
tss_info info;

void tss_init()
{
    info.size = sizeof(tss_entry) * TSS_SIZE - 1;
    info.offset = (uint32_t)&tss;

    __asm__("ltr %0" ::"m"(info));
}