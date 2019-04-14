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

char *__itoa(unsigned int value, char *buffer, int rad)
{
    unsigned int numbers[32];
    int length = 0;

    buffer[0] = 0;

    while (value != 0)
    {
        numbers[length] = value % 16;
        value /= 16;
        length++;
    }

    for (int i = length - 1; i >= 0; i--)
    {
        buffer[length - 1 - i] = numbers[i] >= 10 ? numbers[i] - 10 + 'a' : numbers[i] + '0';
    }

    if (length == 0)
    {
        buffer[0] = '0';
        length++;
    }

    buffer[length] = 0;

    return buffer;
}

void memory_map_dump()
{
    vga_printstring("BaseAddress  Length  Type  ACPI\n");

    for (int i = 0; i < *memory_map_entries_count; i++)
    {
        char buffer[40];

        __itoa(memory_map[i].base_address >> 32, buffer, 16);
        vga_printstring(buffer);

        __itoa(memory_map[i].base_address & 0xffffffff, buffer, 16);
        vga_printstring(buffer);

        vga_printstring(" ");

        __itoa(memory_map[i].length >> 32, buffer, 16);
        vga_printstring(buffer);

        __itoa(memory_map[i].length & 0xffffffff, buffer, 16);
        vga_printstring(buffer);

        vga_printstring(" ");

        __itoa(memory_map[i].type, buffer, 10);
        vga_printstring(buffer);

        vga_printstring(" ");

        __itoa(memory_map[i].acpi, buffer, 10);
        vga_printstring(buffer);

        vga_newline();
    }
}