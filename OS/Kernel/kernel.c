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
#include "FileSystems/filesystem.h"
#include "Panic/panic_screen.h"
#include "Process/ELF/Parser/elf_parser.h"
#include "Process/ELF/Loader/elf_loader.h"
#include "Process/Manager/process_manager.h"
#include "Interrupts/Syscalls/syscalls_manager.h"
#include "TSS/tss.h"
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

    heap_set_kernel_heap((void *)((uint32_t)774 * 1024 * 1024 * 4));
    heap_init_kernel_heap();
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

    tss_init();
    logger_log_ok("TSS");

    syscalls_manager_init();
    logger_log_ok("Syscalls manager");

    /*pci_init();
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
    pci_dev* dev = get_device(0);
    log_info(itoa(dev->vendor_id, buff, 16));
    log_info(itoa(dev->header_type, buff, 16));
    log_info(itoa(dev->class_code, buff, 16));
    log_info(itoa(dev->subclass, buff, 16));
    log_info(itoa(dev->prog_if, buff, 16));*/
    fat_init();
    logger_log_ok("FAT12");

    process_manager_init();
    logger_log_ok("Process manager");

    logger_log_info("MicrOS ready");
    logger_log_info("Created by Application Section of SKNI KOD");
    logger_log_info("Development version");
}

void clear_bss()
{
    extern uint32_t BSS_SECTION_START;
    extern uint32_t BSS_SECTION_END;

    void *bss_start_addr = &BSS_SECTION_START;
    void *bss_end_addr = &BSS_SECTION_END;
    uint32_t bss_length = bss_end_addr - bss_start_addr;

    memset(bss_start_addr, 0, bss_length);
}

int kmain()
{
    clear_bss();

    startup();
    logger_log_info("Hello, World!");
    //startup_music_play();
    logger_log_ok("READY.");

    //process_manager_create_process("/ENV/TASKS.ELF", "Honoka Kotori");
    /*process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");
    process_manager_create_process("/ENV/SHELL.ELF", "Umi Rin");
    process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");
    process_manager_create_process("/ENV/SHELL.ELF", "Umi Rin");
    process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");
    process_manager_create_process("/ENV/SHELL.ELF", "Umi Rin");
    process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");
    process_manager_create_process("/ENV/SHELL.ELF", "Umi Rin");
    process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");
    process_manager_create_process("/ENV/SHELL.ELF", "Umi Rin");
    process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");
    process_manager_create_process("/ENV/SHELL.ELF", "Umi Rin");
    process_manager_run();*/

    /*while (1)
        ;
    */
    
    while (1)
    {
        if (!keyboard_is_buffer_empty())
        {
            keyboard_scan_ascii_pair c;
            keyboard_get_key_from_buffer(&c);
            if (c.scancode == 59)
            {
                if (getMode() != 3)
                {
                    set3HVideoMode();
                    //printRegs();
                }
            }
            else if (c.scancode == 60)
            {
                if (getMode() != 0x13)
                {
                    set13HVideoMode();
                    drawDupaIn13H(0);
                    //test13H();
                    drawMicrOSLogoIn13H();
                    //dumpRegs();
                    //drawDupaIn13H(0);
                }
            }
            else if (c.scancode == 61)
            {
                //pc_speaker_enable_sound(1000);
                if (getMode() != 0x69)
                {
                    setModeYVideoMode();
                    drawDupaInY(0x3);
                    //pixel_11H(1, 5, 0);
                }
            }
            else if (c.scancode == 62)
            {
                if (getMode() != 0x12)
                {
                    set12HVideoMode();
                    drawDupaIn12H(0x6);
                    //pixel_11H(1, 5, 0);
                }
            }
            else if (c.scancode == 63)
            {
                if (getMode() != 0x11)
                {
                    set11HVideoMode();
                    //drawMicrOSLogoIn11H();
                    drawDupaIn11H(0x6);
                    //pixel_11H(1, 5, 0);
                }
                //panic_screen_show(NULL, 0x42, "Someone ask Question about Life, the Universe and Evertything");
            }
            else if (c.scancode == 64)
            {
                if (getMode() != 0x5)
                {
                    set5HVideoMode();
                    drawDupaIn5H(0x5);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else if (c.scancode == 65)
            {
                if (getMode() != 0x4)
                {
                    set4HVideoMode();
                    drawDupaIn4H(0x2);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else if (c.scancode == 66)
            {
                if (getMode() != 0x6)
                {
                    set6HVideoMode();
                    drawDupaIn6H(0x1);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else if (c.scancode == 67)
            {
                if (getMode() != 0xd)
                {
                    setDHVideoMode();
                    drawDupaInDH(0x1);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else if (c.scancode == 68)
            {
                if (getMode() != 0xe)
                {
                    setEHVideoMode();
                    drawDupaInEH(0x4);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else if (c.scancode == 87)
            {
                if (getMode() != 0x10)
                {
                    set10HVideoMode();
                    drawDupaIn10H(0xa);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else if (c.scancode == 88)
            {
                if (getMode() != 0xf)
                {
                    setFHVideoMode();
                    drawDupaInFH(0x2);
                    //pixel_11H(1, 5, 0);
                }
                //memoryViewer();
            }
            else
            vga_printchar(c.ascii);
        }
    }


    return 0;
}