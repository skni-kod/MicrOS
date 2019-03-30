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
#include "Drivers/DAL/VideoCard/videocard.h"
#include "Drivers/VGA/genericvga.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

char buff[50];

void startup()
{
    // Must be done before any VGA operation
    gdt_init();
    paging_init();

    //Don't use VGA before calling VGA init
    vga_init();
    logger_log_info("MicrOS is starting...");
    logger_log_ok("BASIC TEXT VGA Driver");

    //Loading Generic VGA Driver
    driverInit();
    logger_log_ok("Loaded DAL, and Generic VGA Driver");

    physical_memory_init();
    logger_log_ok("Physical Memory");

    heap_set_kernel_heap((void *)((uint32_t)774 * 1024 * 1024 * 4));
    heap_init_kernel_heap();
    logger_log_ok("Virtual Memory");

    idt_init();
    logger_log_ok("Interrupt Descriptor Table");

    pic_init();
    logger_log_ok("Programmable Interrupt Controller");

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

    /*logger_log_ok("Loading tasks...");
    process_manager_create_process("/ENV/TASKS.ELF", "Honoka Kotori");

    logger_log_ok("Loading shell...");
    process_manager_create_process("/ENV/SHELL.ELF", "Nozomi Eli");

    logger_log_ok("Loading shell...");
    process_manager_create_process("/ENV/SHELL.ELF", "Nico Maki");

    process_manager_run();*/

    //while (1)
    //    ;
    char buff[50];
    VideoMode currentMode;
    srand(clock());
    while (1)
    {
        if (!keyboard_is_buffer_empty())
        {
            keyboard_scan_ascii_pair c;
            keyboard_get_key_from_buffer(&c);
            if(c.scancode == 59) //F1
            {
                    setVideoMode(0x3);
                    logger_log_ok("UDALO SIE WSKOCZYC na 0x03\n");
                    currentMode = getCurrentVideoMode();
                    vga_printstring("Szerokosc Trybu: ");
                    itoa(currentMode.width, buff, 10);
                    vga_printstring(buff);
                    vga_newline();
                    vga_printstring("Wysokosc Trybu: ");
                    itoa(currentMode.height, buff, 10);
                    vga_printstring(buff);
                    vga_newline();
                    vga_printstring("Ilosc Kolorow: ");
                    itoa(currentMode.colors, buff, 10);
                    vga_printstring(buff);
                    vga_newline();
                    vga_printstring("Monochromatyczny: ");
                    itoa(currentMode.monochrome, buff, 10);
                    vga_printstring(buff);
                    vga_newline();
                    vga_printstring("Pamiec platowa: ");
                    itoa(currentMode.planar, buff, 10);
                    vga_printstring(buff);
                    vga_newline();
                    vga_printstring("-------TESTY INNYCH FUNKCJI-------:\n");
                    vga_printstring("Czy tekstowy: ");
                    itoa(isTextMode(), buff, 10);
                    vga_printstring(buff);
                    vga_newline();
                    
            }
            else if(c.scancode == 60) //F2
            {
                setVideoMode(0x04);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
            }
            else if(c.scancode == 2) //1
            {
                setVideoMode(0x04);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x02, x, y);
                swapBuffers();
            }
            else if(c.scancode == 61) //F3
            {
                setVideoMode(0x05);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x03, x, y);
            }
            else if(c.scancode == 3) //2
            {
                setVideoMode(0x05);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
                swapBuffers();
            }
            else if(c.scancode == 62) //F4
            {
                setVideoMode(0x06);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
            }
            else if(c.scancode == 4) //3
            {
                setVideoMode(0x06);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
                swapBuffers();
            }
            else if(c.scancode == 63) //F5
            {
                setVideoMode(0x0d);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x04, x, y);
            }
            else if(c.scancode == 5) //4
            {
                setVideoMode(0x0d);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x05, x, y);
                swapBuffers();
            }
            else if(c.scancode == 64) //F6
            {
                setVideoMode(0x0e);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x06, x, y);
            }
            else if(c.scancode == 6) //5
            {
                setVideoMode(0x0e);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x07, x, y);
                swapBuffers();
            }
            else if(c.scancode == 65) //F7
            {
                setVideoMode(0x0f);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
            }
            else if(c.scancode == 7) //6
            {
                setVideoMode(0x0f);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x02, x, y);
                swapBuffers();
            }
            else if(c.scancode == 66) //F8
            {
                setVideoMode(0x10);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x08, x, y);
            }
            else if(c.scancode == 8) //7
            {
                setVideoMode(0x10);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x09, x, y);
                swapBuffers();
            }
            else if(c.scancode == 67) //F9
            {
                setVideoMode(0x11);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
            }
            else if(c.scancode == 9) //8
            {
                setVideoMode(0x11);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x01, x, y);
                swapBuffers();
            }
            else if(c.scancode == 68) //F10
            {
                setVideoMode(0x12);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x0A, x, y);
            }
            else if(c.scancode == 10) //9
            {
                setVideoMode(0x12);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x0B, x, y);
                swapBuffers();
            }
            else if(c.scancode == 87) //F11
            {
                setVideoMode(0x13);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x10, x, y);
            }
            else if(c.scancode == 11) //0
            {
                setVideoMode(0x13);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x25, x, y);
                swapBuffers();
            }
            else if(c.scancode == 88) //F12
            {
                setVideoMode(0x69);
                currentMode = getCurrentVideoMode();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x45, x, y);
            }
            else if(c.scancode == 12) //-
            {
                setVideoMode(0x69);
                currentMode = getCurrentVideoMode();
                turnOnBuffer();
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(0x8, x, y);
                swapBuffers();
            }
            else if(c.scancode == 01) //ESC
            {
                clearScreen();
            }
            else if(c.scancode == 41) // `
            {
                currentMode = getCurrentVideoMode();
                uint8_t color = (rand() % (currentMode.colors - 1) + 1);
                for(uint32_t x = 0; x < currentMode.width; x++)
                    for(uint32_t y = 0; y < currentMode.height; y++)
                        drawPixel(color, x, y);
            }
            else if(c.scancode == 26) //[
            {
                turnOnBuffer();
            }
            else if(c.scancode == 27) // ]
            {
                turnOffBuffer();
            }
            else{
                if(isTextMode())
                    vga_printchar(c.ascii);
            }
        }
    }

    return 0;
}