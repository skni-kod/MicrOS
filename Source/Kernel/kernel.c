#include "Drivers/VGA/vga.h"
#include "Interrupts/PIC/pic.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Interrupts/IDT/idt.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Drivers/VGA/vga_gmode.h"
#include "Drivers/PCSpeaker/pcspeaker.h"
#include "Misc/startupMisc.h"
#include "Timer/timer.h"
#include <stdint.h>

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
    //Don't use VGA before calling VGA init
    vga_init();
    vga_printstring("MicrOS is starting...\n");
    print_ok_status("VGA Driver");

    pic_init();
    print_ok_status("Programmable Interrupt Controller");

    idt_init();
    print_ok_status("Interrupt Descriptor Table");
    
    keyboard_init();
    print_ok_status("Keyboard");

    timer_init();
    print_ok_status("Timer");

    vga_printstring("MicrOS ready\n");
    vga_printstring("Created by Application Section of SKNI KOD\n");
    vga_printstring("Version ... no version\n");
}

int kmain()
{
    vga_color col;
    col.color_without_blink.letter = VGA_COLOR_GREEN;
    col.color_without_blink.background = VGA_COLOR_BLACK;
    startup();
    vga_printstring("Hello, World!\n");
    whatIsLove();
    vga_printstring("\nREADY.\n");

    while(1)
    {
        if(!isBufferEmpty())
        {
            ScanAsciiPair c = get_key_from_buffer();
            if(c.scancode == 59)
            {
                if(getMode() != 3)
                    set3HVideoMode();
            }
            else if(c.scancode == 60)
            {
                if(getMode() != 0x13)
                {
                    set13HVideoMode();
                    drawDupaIn13H(10);
                }
            }
            else if(c.scancode == 61)
            {
                sound(1000);
            }
            else if(c.scancode == 62)
            {
                nosound();
            }
            else
                vga_printchar(c.ascii);
        }
    }

    return 0;
}