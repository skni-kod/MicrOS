#include "Drivers/VGA/vga.h"
#include "Interrupts/PIC/pic.h"
#include "Interrupts/IDT/idt.h"

void print_ok_status(char* message)
{
    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_GREEN;
    col.color_without_blink.background = VGA_COLOR_BLACK;
    
    vga_printstring("[ ");
    vga_printstring_color("OK", &col);
    vga_printstring(" ] ");
    vga_printstring(message);
    vga_printstring("\n");
}

void startup()
{
    vga_clear_screen();
    vga_printstring("MicrOS is starting...\n");
    print_ok_status("VGA Driver");

    pic_init();
    print_ok_status("Programmable Interrupt Controller");

    idt_init();
    print_ok_status("Interrupt Descriptor Table");

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