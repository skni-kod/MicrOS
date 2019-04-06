#include "physical_memory_manager.h"

physical_memory_entry physical_entries[1024];
physical_memory_entry mem_entries[1024];
char buffer[500];

int between(uint64_t number, uint64_t a, uint64_t b);
int between_or_eq(uint64_t number, uint64_t a, uint64_t b);

void physical_memory_init()
{
    memory_map_entry *memory_map_entries = memory_map_get();
    uint8_t entries_count = memory_map_get_entries_count();

    // Set all entries as not available
    for (int i = 0; i < 1024; i++)
    {
        physical_entries[i].type = physical_memory_not_available;
    }

    // Mark entries if they are free or reserved (due to BIOS memory table)
    for (int i = 0; i < entries_count; i++)
    {
        uint16_t start_index = memory_map_entries[i].base_address / 1024 / 1024 / 4;
        uint16_t end_index = start_index + (memory_map_entries[i].length / 1024 / 1024 / 4);

        for (int x = start_index; x <= end_index; x++)
        {
            if (memory_map_entries[i].type != memory_map_free)
            {
                physical_entries[x].type = physical_memory_reserved;
            }
            else if (physical_entries[x].type == physical_memory_not_available)
            {
                physical_entries[x].type = physical_memory_free;
            }
        }
    }

    // Mark first 24 megabytes as reserved
    for (int i = 0; i < 6; i++)
    {
        physical_entries[i].type = physical_memory_reserved;
    }
}

uint32_t physical_memory_alloc_page()
{
    for (int i = 0; i < 1024; i++)
    {
        if (physical_entries[i].type == physical_memory_free)
        {
            physical_entries[i].type = physical_memory_filled;
            return i;
        }
    }

    return 0;
}

bool physical_memory_dealloc_page(uint32_t index)
{
    if (physical_entries[index].type == physical_memory_filled)
    {
        physical_entries[index].type = physical_memory_free;
        return false;
    }

    return true;
}

void physical_memory_dump()
{
    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_WHITE;

    for (int i = 0; i < 1024; i++)
    {
        switch (physical_entries[i].type)
        {
        case physical_memory_free:
            col.color_without_blink.background = VGA_COLOR_GREEN;
            break;
        case physical_memory_reserved:
            col.color_without_blink.background = VGA_COLOR_BLUE;
            break;
        case physical_memory_filled:
            col.color_without_blink.background = VGA_COLOR_RED;
            break;
        case physical_memory_not_available:
            col.color_without_blink.background = VGA_COLOR_DARK_GRAY;
            break;
        }

        vga_printchar_color(' ', &col);
    }

    vga_newline();
}

void draw_4MB_array(uint64_t number_of_sector)
{
    uint64_t begin_addr = number_of_sector * 1024 * 1024 * 4;
    uint64_t end_addr = begin_addr + 1024 * 4;

    memory_map_entry *memory_map_entries = memory_map_get();
    uint8_t entries_count = memory_map_get_entries_count();

    for (int i = 0; i < 1024; i++)
    {
        mem_entries[i].type = physical_memory_not_available;
    }

    for (int i = 0; i < entries_count; i++)
    {
        uint64_t start_index = memory_map_entries[i].base_address;
        uint64_t end_index = start_index + (memory_map_entries[i].length);

        if ((begin_addr <= start_index) && (end_addr <= end_index))
        {
            uint64_t a = (start_index - begin_addr) / 1024 / 4;
            uint64_t b = (end_index - begin_addr) / 1024 / 4;

            for (; a < b; a++)
            {
                if (memory_map_entries[i].type != 1)
                {
                    mem_entries[a].type = physical_memory_reserved;
                }
                else if (mem_entries[a].type == physical_memory_not_available)
                {
                    mem_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }

        if ((start_index < begin_addr) && between_or_eq(end_index, begin_addr, end_addr))
        {
            uint64_t a = 0;
            uint64_t b = (end_index - begin_addr) / 1024 / 4;

            for (; a < b; a++)
            {
                if (memory_map_entries[i].type != 1)
                {
                    mem_entries[a].type = physical_memory_reserved;
                }
                else if (mem_entries[a].type == physical_memory_not_available)
                {
                    mem_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }

        if (between_or_eq(start_index, begin_addr, end_addr) && (end_index > end_addr))
        {
            uint64_t a = (start_index - begin_addr) / 1024 / 4;
            uint64_t b = 1024;

            for (; a < b; a++)
            {
                if (memory_map_entries[i].type != 1)
                {
                    mem_entries[a].type = physical_memory_reserved;
                }
                else if (mem_entries[a].type == physical_memory_not_available)
                {
                    mem_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }

        if ((start_index < begin_addr) && (end_index > end_addr))
        {
            for (int a = 0; a < 1024; a++)
            {
                if (memory_map_entries[i].type != 1)
                {
                    mem_entries[a].type = physical_memory_reserved;
                }
                else if (mem_entries[a].type == physical_memory_not_available)
                {
                    mem_entries[a].type = physical_memory_free;
                }
            }
            continue;
        }
    }

    vga_color col;
    //col.color_without_blink.letter = VGA_COLOR_WHITE;

    for (int i = 0; i < 1024; i++)
    {
        switch (mem_entries[i].type)
        {
        case physical_memory_free:
            col.color_without_blink.letter = VGA_COLOR_GREEN;
            break;
        case physical_memory_reserved:
            col.color_without_blink.letter = VGA_COLOR_BLUE;
            break;
        case physical_memory_filled:
            col.color_without_blink.letter = VGA_COLOR_RED;
            break;
        case physical_memory_not_available:
            col.color_without_blink.letter = VGA_COLOR_DARK_GRAY;
            break;
        }
        if (i % 2)
        {
            col.color_without_blink.background = VGA_COLOR_BLACK;
            vga_printchar_color(178, &col);
        }
        else
        {
            col.color_without_blink.background = col.color_without_blink.letter;
            col.color_without_blink.letter = VGA_COLOR_BLACK;
            vga_printchar_color('_', &col);
        }
    }

    vga_newline();
}

int between(uint64_t number, uint64_t a, uint64_t b)
{
    if ((a < number) && (number < b))
        return 1;
    return 0;
}

int between_or_eq(uint64_t number, uint64_t a, uint64_t b)
{
    if ((a <= number) && (number <= b))
        return 1;
    return 0;
}

void drawInVGA(uint64_t i)
{
    vga_clear_screen();
    vga_printstring("Memory Viewer\n\n");
    draw_4MB_array(i);
    vga_newline();
    vga_newline();
    vga_printstring("Showing: ");
    vga_printstring(itoa(i, buffer, 10));
    vga_printstring(" memory sector (4MiB).\nEach char is one page width (4KiB).\n");
    vga_printstring("Begin address of watching memory: 0x");
    vga_printstring(itoa(i * 4 * 1024 * 1024, buffer, 16));
    vga_printstring(" (");
    vga_printstring(itoa(i * 4, buffer, 10));
    vga_printstring(" MiB)\nEnd address of watching memory: 0x");
    vga_printstring(itoa((i + 1) * 4 * 1024 * 1024, buffer, 16));
    vga_printstring(" (");
    vga_printstring(itoa((i + 1) * 4, buffer, 10));
    vga_printstring(" MiB)\n");
    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_WHITE;
    col.color_without_blink.background = VGA_COLOR_GREEN;
    vga_printchar_color(' ', &col);
    vga_printstring(" - free,  ");
    col.color_without_blink.background = VGA_COLOR_BLUE;
    vga_printchar_color(' ', &col);
    vga_printstring(" - reserved,  ");
    col.color_without_blink.background = VGA_COLOR_RED;
    vga_printchar_color(' ', &col);
    vga_printstring(" - allocated,  ");
    col.color_without_blink.background = VGA_COLOR_DARK_GRAY;
    vga_printchar_color(' ', &col);
    vga_printstring(" - no access\n");
    vga_printstring("F1 - previous sector, F2 - next sector, F3 - prev 100 sect, F4 - next 100 sect\n");
    vga_printstring("ESC - quit");
}

void memoryViewer()
{
    uint64_t i = 0;

    drawInVGA(i);
    while (1)
    {
        if (!keyboard_is_buffer_empty())
        {
            keyboard_scan_ascii_pair c;
            keyboard_get_key_from_buffer(&c);

            if (c.scancode == 60)
            {
                i++;
                drawInVGA(i);
            }
            else if (c.scancode == 59)
            {
                i--;
                drawInVGA(i);
            }
            if (c.scancode == 62)
            {
                i += 100;
                drawInVGA(i);
            }
            else if (c.scancode == 61)
            {
                i -= 100;
                drawInVGA(i);
            }
            else if (c.scancode == 1)
            {
                vga_clear_screen();
                return;
            }
        }
    }
}

void physical_memory_get_stats(physical_memory_stats *stats)
{
    stats->free_entries = 0;
    stats->reserved_entries = 0;
    stats->allocated_entries = 0;

    for (int i = 0; i < 1024; i++)
    {
        switch (physical_entries[i].type)
        {
        case physical_memory_free:
            stats->free_entries++;
            break;
        case physical_memory_reserved:
            stats->reserved_entries++;
            break;
        case physical_memory_filled:
            stats->allocated_entries++;
            break;
        }
    }
}