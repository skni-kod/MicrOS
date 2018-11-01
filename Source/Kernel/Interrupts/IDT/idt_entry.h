#pragma once
#include <stdint.h>

struct IDT_Entry {
    uint16_t offset_0_15;
    uint16_t selector;
    uint8_t reserved;
    uint8_t type : 8;
    uint8_t storage_segment : 1;
    uint8_t privilege_level : 2;
    uint8_t present : 1;
    uint8_t offset_16_31;
}