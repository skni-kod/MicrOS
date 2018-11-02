#include "Drivers/VGA/vga.h"

void startup()
{
    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_GREEN;
    col.color_without_blink.background = VGA_COLOR_BLACK;
    vga_clear_screen();
    vga_printstring("MicrOS is starting...\n");
    vga_printstring("[ ");
    vga_printstring_color("OK", &col);
    vga_printstring(" ] VGA driver\n");
    vga_printstring("MicrOS ready\n");
    vga_printstring("Created by Application Section of SKNI KOD\n");
    vga_printstring("Version ... no version\n");
}

int kmain()
{
    startup();
    vga_printstring("Hello, World!\n");

    vga_printstring("\nREADY.");
    return 0;
}