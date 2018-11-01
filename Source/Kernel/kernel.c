#include "Drivers/VGA/vga.h"
#include "Interrupts/IDT/idt.h"
#include "Interrupts/PIC/pic.h"
#include "Assembly/io.h"

int kmain()
{
    pic_init();
    idt_init();

    // Enable keyboard
    outb(0x21,0xfd);
    outb(0xa1,0xff);
    enable();

    vga_printstring("Hello, World!");

    while(1) {}
    return 0;
}