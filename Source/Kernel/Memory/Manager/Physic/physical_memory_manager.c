#include "physical_memory_manager.h"

physical_memory_entry physical_entries[1024];

int between(uint64_t number, uint64_t a, uint64_t b);

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

void draw_4MB_array(uint64_t number_of_sector)
{
    uint64_t begin_addr = number_of_sector * 1024 * 1024 * 4;
    uint64_t end_addr = begin_addr + 1024 * 4;
    physical_memory_entry mem_entries[1024];

    memory_map_entry* memory_map_entries = memory_map_get();
    uint8_t entries_count = memory_map_get_entries_count();

    for(int i=0; i<1024; i++)
    {
        mem_entries[i].type = physical_memory_not_available;
    }

    for(int i=0; i<entries_count; i++)
    {
        uint64_t start_index = memory_map_entries[i].base_address;
        uint64_t end_index = start_index + (memory_map_entries[i].length);

        if((begin_addr < start_index) && (end_addr < end_index))
        {
            uint64_t a = (start_index - begin_addr) / 1024 / 4;
            uint64_t b = (end_index - begin_addr) / 1024 / 4;

            for(; a<b; a++)
            {
                if(memory_map_entries[i].type != 1)
                {
                    physical_entries[a].type = physical_memory_reserved;
                }
                else if(physical_entries[a].type == physical_memory_not_available)
                {
                    physical_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }

        if((start_index < begin_addr) && between(end_index, begin_addr, end_addr))
        {
            uint64_t a = 0;
            uint64_t b = (end_index - begin_addr) / 1024 / 4;

            for(; a<b; a++)
            {
                if(memory_map_entries[i].type != 1)
                {
                    physical_entries[a].type = physical_memory_reserved;
                }
                else if(physical_entries[a].type == physical_memory_not_available)
                {
                    physical_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }

        if(between(start_index, begin_addr, end_addr) && (end_index > end_addr))
        {
            uint64_t a = (start_index - begin_addr) / 1024 / 4;
            uint64_t b = 1024;

            for(; a<b; a++)
            {
                if(memory_map_entries[i].type != 1)
                {
                    physical_entries[a].type = physical_memory_reserved;
                }
                else if(physical_entries[a].type == physical_memory_not_available)
                {
                    physical_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }

        if((start_index < begin_addr) && (end_index > end_addr))
        {
            for(int a = 0; a<1024; a++)
            {
                if(memory_map_entries[i].type != 1)
                {
                    physical_entries[a].type = physical_memory_reserved;
                }
                else if(physical_entries[a].type == physical_memory_not_available)
                {
                    physical_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }
    }

    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_WHITE;

    for(int i=0; i<1024; i++)
    {
        switch(mem_entries[i].type)
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

int between(uint64_t number, uint64_t a, uint64_t b)
{
    if((a < number) && (number < b))
        return 1;
    return 0;
}

void memoryViewer()
{
    uint64_t i = 0;
    vga_clear_screen();
    vga_printstring("Memory Viewer\n\n");
    draw_4MB_array(i);

    while(1)
    {
        if(!isBufferEmpty())
        {
            ScanAsciiPair c = get_key_from_buffer();
            if(c.scancode == 59)
            {
                i++;
                vga_clear_screen();
                vga_printstring("Memory Viewer\n\n");
                draw_4MB_array(i);
            }
            else if(c.scancode == 60)
            {
                i--;
                vga_clear_screen();
                vga_printstring("Memory Viewer\n\n");
                draw_4MB_array(i);
            }
            else if(c.scancode == 1)
            {
                vga_clear_screen();
                return;
            }
        }
    }
}