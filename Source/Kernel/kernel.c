#include "Drivers/VGA/vga.h"
#include "Interrupts/IDT/idt.h"
#include "Interrupts/PIC/pic.h"

void startup()
{
    outb(0x21,0xfd);
    outb(0xa1,0xff);
    enable();  
    vga_color col;
    col.vga_color_without_blink.letter = VGA_COLOR_GREEN;
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
    pic_init();
    idt_init();
    startup();
    vga_printstring("Hello, World!\n");

    vga_printstring("\nREADY.");
    return 0;
}