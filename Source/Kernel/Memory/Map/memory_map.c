#include "memory_map.h"

memory_map_entry* memory_map = MEMORY_MAP_ADDRESS;

void memory_map_dump()
{
    

    for(int i=0; i<10; i++)
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