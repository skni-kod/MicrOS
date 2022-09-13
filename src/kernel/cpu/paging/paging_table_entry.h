#ifndef PAGE_TABLE_ENTRY_H
#define PAGE_TABLE_ENTRY_H

#include <stdint.h>

typedef struct paging_table_entry
{
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_supervisor : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t reserved : 1;
    uint8_t dirty : 1;
    uint8_t global : 1;

    uint8_t available : 3;
    uint32_t physical_page_address : 20;
} __attribute__((packed)) paging_table_entry;

#endif