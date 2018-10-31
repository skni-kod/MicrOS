#include "vga.h"

int cursor_pos = 0;

void vga_printchar(char c)
{
    volatile char* video = (volatile char*)(VGA_BASE_ADDR + cursor_pos);
    *video = c;

    cursor_pos += 2;
}

void vga_printstring(char* str)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar(*ptr);
        ptr++;
    }
}