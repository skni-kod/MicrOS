#include "Drivers/VGA/vga.h"
#include "Interrupts/PIC/pic.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Interrupts/IDT/idt.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Drivers/Floppy/floppy.h"
#include "Drivers/VGA/vga_gmode.h"
#include "Drivers/PCSpeaker/pcspeaker.h"
#include "Misc/startupMisc.h"
#include "Timer/timer.h"
#include "Logger/logger.h"
#include "Memory/GDT/gdt.h"
#include "Memory/Paging/paging.h"
#include "Memory/Manager/memory_manager.h"
#include <stdint.h>

void startup()
{
    // Must be done before any VGA operation
    gdt_init();
    paging_init();

    //Don't use VGA before calling VGA init
    vga_init();
    log_info("MicrOS is starting...");
    log_ok("VGA Driver");
    
    memory_manager_set_base_address(0xc0100000);
    memory_manager_init_memory(0x30000);
    log_ok("Memory manager");

    pic_init();
    log_ok("Programmable Interrupt Controller");

    idt_init();
    log_ok("Interrupt Descriptor Table");

    timer_init();
    log_ok("Timer");

    floppy_init();
    log_ok("Floppy");
    
    keyboard_init();
    log_ok("Keyboard");

    timer_init();
    log_ok("Timer");

    log_info("MicrOS ready");
    log_info("Created by Application Section of SKNI KOD");
    log_info("Version ... no version");
}

int kmain()
{
    startup();

    log_info("Hello, World!");
    //whatIsLove();
    log_ok("READY.");

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