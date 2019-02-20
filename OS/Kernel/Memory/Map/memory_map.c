#include "memory_map.h"

uint8_t *memory_map_entries_count = (uint8_t *)MEMORY_MAP_ENTRIES_COUNT;
memory_map_entry *memory_map = (memory_map_entry *)(MEMORY_MAP_BUFFER);

memory_map_entry *memory_map_get()
{
    return memory_map;
}

uint8_t memory_map_get_entries_count()
{
    return *memory_map_entries_count;
}

void memory_map_dump()
{
    vga_printstring("BaseAddress  Length  Type  ACPI\n");

    for (int i = 0; i < *memory_map_entries_count; i++)
    {
        char buffer[40];

        itoa(memory_map[i].base_address >> 32, buffer, 16);
        vga_printstring(buffer);

        itoa(memory_map[i].base_address & 0xffffffff, buffer, 16);
        vga_printstring(buffer);

        vga_printstring(" ");

        itoa(memory_map[i].length >> 32, buffer, 16);
        vga_printstring(buffer);

        itoa(memory_map[i].length & 0xffffffff, buffer, 16);
        vga_printstring(buffer);

        vga_printstring(" ");

        itoa(memory_map[i].type, buffer, 10);
        vga_printstring(buffer);

        vga_printstring(" ");

        itoa(memory_map[i].acpi, buffer, 10);
        vga_printstring(buffer);

        vga_newline();
    }
}