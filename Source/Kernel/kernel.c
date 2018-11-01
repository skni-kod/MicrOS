#include "Drivers/VGA/vga.h"
#include "Interrupts/IDT/idt.h"

int kmain()
{
    idt_init();
    vga_printstring("Hello, World!");
    return 0;
}