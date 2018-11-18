#include "Drivers/VGA/vga.h"
#include "Interrupts/PIC/pic.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Interrupts/IDT/idt.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Drivers/Floppy/floppy.h"
#include "Drivers/VGA/vga_gmode.h"
#include "Drivers/PCSpeaker/pcspeaker.h"
#include "Drivers/RTC/RTC.h"
#include "Misc/startupMisc.h"
#include "Timer/timer.h"
#include "Logger/logger.h"
#include "Memory/GDT/gdt.h"
#include "Memory/Paging/paging.h"
#include "Memory/Map/memory_map.h"
#include "Memory/Manager/Physic/physical_memory_manager.h"
#include "Misc/panicScreen.h"
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

    memory_map_dump();

    physical_memory_init();
    physical_memory_dump();

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
            else if(c.scancode == 63)
            {
                showPanicScreen(0x42, "Someone ask Question about Life, the Universe and Evertything");
            }
            else if(c.scancode == 64)
            {
                memoryViewer();
            }
            else
                vga_printchar(c.ascii);
        }
    }

    return 0;
}