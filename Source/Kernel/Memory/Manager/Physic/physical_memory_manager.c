#include "physical_memory_manager.h"

physical_memory_entry physical_entries[1024];

void physical_memory_init()
{
    memory_map_entry* memory_map_entries = memory_map_get();
    uint8_t entries_count = memory_map_get_entries_count();

    for(int i=0; i<1024; i++)
    {
        physical_entries[i].type = physical_memory_not_available;
    }

    for(int i=0; i<entries_count; i++)
    {
        uint16_t start_index = memory_map_entries[i].base_address / 1024 / 1024 / 4;
        uint16_t end_index = start_index + (memory_map_entries[i].length / 1024 / 1024 / 4);

        for(int x=start_index; x<=end_index; x++)
        {
            if(memory_map_entries[i].type != 1)
            {
                physical_entries[x].type = physical_memory_reserved;
            }
            else if(physical_entries[x].type == physical_memory_not_available)
            {
                physical_entries[x].type = physical_memory_free;
            }
        }
    }
}

void physical_memory_dump()
{
    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_WHITE;

    for(int i=0; i<1024; i++)
    {
        switch(physical_entries[i].type)
        {
            case physical_memory_free:          col.color_without_blink.background = VGA_COLOR_GREEN; break;
            case physical_memory_reserved:      col.color_without_blink.background = VGA_COLOR_BLUE; break;
            case physical_memory_filled:        col.color_without_blink.background = VGA_COLOR_RED; break;
            case physical_memory_not_available: col.color_without_blink.background = VGA_COLOR_DARK_GRAY; break;
        }

        vga_printchar_color(' ', &col);
    }

    vga_newline();
}