#include "Drivers/VGA/vga.h"
#include "Interrupts/IDT/idt.h"
#include "Interrupts/PIC/pic.h"
#include "Assembly/io.h"

int kmain()
{
    pic_init();
    idt_init();

    pic_enable_irq(1);
    vga_printstring("Hello, World!");

    return 0;
}