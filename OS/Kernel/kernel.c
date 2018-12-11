#include "Drivers/VGA/vga.h"
#include "Interrupts/PIC/pic.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Interrupts/IDT/idt.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Drivers/Floppy/floppy.h"
#include "Drivers/VGA/vga_gmode.h"
#include "Drivers/PCSpeaker/pc_speaker.h"
#include "Drivers/PCI/pci.h"
#include "Startup/startup_music.h"
#include "Timer/timer.h"
#include "Logger/logger.h"
#include "Memory/GDT/gdt.h"
#include "Memory/Paging/paging.h"
#include "Memory/Map/memory_map.h"
#include "Memory/Manager/Physic/physical_memory_manager.h"
#include "FileSystem/fat12.h"
#include "Panic/panic_screen.h"
#include <stdint.h>

char buff[50];

void startup()
{
    // Must be done before any VGA operation
    gdt_init();
    paging_init();

    //Don't use VGA before calling VGA init
    vga_init();
    logger_log_info("MicrOS is starting...");
    logger_log_ok("VGA Driver");

    physical_memory_init();
    logger_log_ok("Physical Memory");

    virtual_memory_set_base_page_index(774);
    heap_set_base_page_index(774);
    heap_clear();
    logger_log_ok("Virtual Memory");

    pic_init();
    logger_log_ok("Programmable Interrupt Controller");

    idt_init();
    logger_log_ok("Interrupt Descriptor Table");

    timer_init();
    logger_log_ok("Timer");

    floppy_init();
    logger_log_ok("Floppy");

    keyboard_init();
    logger_log_ok("Keyboard");

    timer_init();
    logger_log_ok("Timer");

    pci_init();
    logger_log_ok("PCI");
    logger_log_info("Number of devices: ");
    uint8_t nd = pci_get_number_of_devices();
    logger_log_info(itoa(nd, buff, 10));
    logger_log_info("Devices: ");
    vga_color col;
    col.color_without_blink.background = VGA_COLOR_BLACK;
    col.color_without_blink.letter = VGA_COLOR_BLUE;
    vga_printstring_color("vendor_id ", &col);
    col.color_without_blink.letter = VGA_COLOR_GREEN;
    vga_printstring_color("device_id ", &col);
    col.color_without_blink.letter = VGA_COLOR_LIGHT_BLUE;
    vga_printstring_color("header_type ", &col);
    col.color_without_blink.letter = VGA_COLOR_LIGHT_RED;
    vga_printstring_color("class_code ", &col);
    col.color_without_blink.letter = VGA_COLOR_YELLOW;
    vga_printstring_color("subclass ", &col);
    col.color_without_blink.letter = VGA_COLOR_MAGENTA;
    vga_printstring_color("prog_if\n", &col);
    for (int i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *dev = pci_get_device(i);
        col.color_without_blink.letter = VGA_COLOR_BLUE;
        vga_printstring_color(itoa(dev->vendor_id, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_GREEN;
        vga_printstring_color(itoa(dev->device_id, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_LIGHT_BLUE;
        vga_printstring_color(itoa(dev->header_type, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_LIGHT_RED;
        vga_printstring_color(itoa(dev->class_code, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_YELLOW;
        vga_printstring_color(itoa(dev->subclass, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_MAGENTA;
        vga_printstring_color(itoa(dev->prog_if, buff, 16), &col);
        vga_printchar('\n');
    }
    /*pci_dev* dev = get_device(0);
    log_info(itoa(dev->vendor_id, buff, 16));
    log_info(itoa(dev->header_type, buff, 16));
    log_info(itoa(dev->class_code, buff, 16));
    log_info(itoa(dev->subclass, buff, 16));
    log_info(itoa(dev->prog_if, buff, 16));*/
    fat12_init();
    logger_log_ok("FAT12");

    logger_log_info("MicrOS ready");
    logger_log_info("Created by Application Section of SKNI KOD");
    logger_log_info("Version ... no version");
}

int kmain()
{
    startup();

    logger_log_info("Hello, World!");
    //startup_music_play();
    logger_log_ok("READY.");

    while (1)
    {
        if (!isBufferEmpty())
        {
            ScanAsciiPair c = get_key_from_buffer();
            if (c.scancode == 59)
            {
                if (getMode() != 3)
                    set3HVideoMode();
            }
            else if (c.scancode == 60)
            {
                if (getMode() != 0x13)
                {
                    set13HVideoMode();
                    drawDupaIn13H(10);
                }
            }
            else if (c.scancode == 61)
            {
                pc_speaker_sound(1000);
            }
            else if (c.scancode == 62)
            {
                pc_speaker_no_sound();
            }
            else if (c.scancode == 63)
            {
                panic_screen_show(0x42, "Someone ask Question about Life, the Universe and Evertything");
            }
            else if (c.scancode == 64)
            {
                memoryViewer();
            }
            else
                vga_printchar(c.ascii);
        }
    }

    return 0;
}