#include "Drivers/VGA/vga.h"

int kmain()
{
    vga_clear_screen();
    vga_printstring("Hello, World!");
    return 0;
}