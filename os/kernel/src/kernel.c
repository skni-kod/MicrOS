#include "drivers/vga/vga.h"
#include "cpu/pic/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "cpu/idt/idt.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/floppy/floppy.h"
#include "drivers/vga/vga_gmode.h"
#include "drivers/pcspeaker/pc_speaker.h"
#include "drivers/pci/pci.h"
#include "init/startup_music.h"
#include "cpu/timer/timer.h"
#include "logger/logger.h"
#include "cpu/gdt/gdt.h"
#include "cpu/paging/paging.h"
#include "memory/map/memory_map.h"
#include "memory/physical/physical_memory_manager.h"
#include "filesystems/filesystem.h"
#include "cpu/panic/panic_screen.h"
#include "process/elf/parser/elf_parser.h"
#include "process/elf/loader/elf_loader.h"
#include "process/manager/process_manager.h"
#include "process/syscalls/syscalls_manager.h"
#include "process/signals/signals_manager.h"
#include "cpu/tss/tss.h"
#include "drivers/dal/videocard/videocard.h"
#include "drivers/vga/genericvga.h"
#include "drivers/vga/modes/mode_13h/mode_13h.h"
#include "drivers/vga/vga_gmode.h"
#include "cpu/dma/dma.h"
#include "drivers/harddisk/harddisk.h"
#include "drivers/harddisk/ata/harddisk_ata.h"
#include "drivers/harddisk/harddisk_identify_device_data.h"
#include "filesystems/partitions/partitions.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "terminal/terminal_manager.h"
#include "cpu/cpuid/cpuid.h"
#include "v8086/v8086.h"
#include "v8086/memory_operations.h"
#include "drivers/vbe/vbe.h"

#include "debug_helpers/library/kernel_stdio.h"

extern terminal_struct* terminals_array;

typedef struct _linesStruct
{
    uint16_t ax;
    uint16_t bx;
    uint16_t ay;
    uint16_t by;
    uint8_t color;
} linesStruct;

char buff[50];
linesStruct ssBuffer[64];

v8086* v8086_machine;

//! Prints processor details.
/*! Used during boot to print informations about print processor.
 */
void print_processor_status()
{
    char vendor_string_buffer[13];
    logger_log_info(cpuid_get_vendor_string(vendor_string_buffer));

    char processor_brand_buffer[48];
    logger_log_info(__cpuid_get_processor_brand(processor_brand_buffer));

    char buff[20];
    char buff2[100];
    uint8_t cores = cpuid_number_of_physical_processors_cores();
    itoa(cores, buff, 20);
    strcpy(buff2, "Number of physical cores: ");
    strcat(buff2, buff);
    logger_log_info(buff2);

    if(cores > 1)
    {
        logger_log_warning("But only one core is used :v");
        logger_log_info("For future SKNI members: add support for more cores");
    }

    for(int i = 0; i < cpuid_get_valid_number_cache_entries(); i++)
    {
        cpuid_cache_struct cache = cpuid_get_cache_data(i);

        strcpy(buff2, "Cache level: ");
        itoa(cache.level, buff, 10);
        strcat(buff2, buff);

        switch(cache.type)
        {
            case DATA_CACHE:
                strcat(buff2, ", type: data, size: ");
                break;
            case INSTRUCTION_CACHE:
                strcat(buff2, ", type: instruction, size: ");
                break;
            case UNIFIED_CACHE:
                strcat(buff2, ", type: unified, size: ");
                break;
        }

        uint32_t size = cache.size / 1024;
        if(size < 1024)
        {
            itoa(size, buff, 10);
            strcat(buff2, buff);
            strcat(buff2, " KiB");
        }
        else
        {
            itoa(size / 1024, buff, 10);
            strcat(buff2, buff);
            strcat(buff2, " MiB");
        }

        logger_log_info(buff2);
    }
}

//! Prints hard disk detail.
/*! Used during boot to print informations about hard disk.
    \param type Type of hard disk.
    \param bus Type of bus for hard disk.
    \param name Name for hard disk eg. "Primary Master", that is printed during boot to specify disk.
 */
void print_harddisk_details(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char* name)
{
    char buff[50];
    char buff2[100];
    HARDDISK_STATE state = harddisk_get_state(type, bus);

    if(state == HARDDISK_ATA_PRESENT)
    {
        strcpy(buff2, name);
        strcat(buff2, ": ATA device");
        logger_log_info(buff2);

        harddisk_get_disk_model_number_terminated(type, bus, buff);
        strcpy(buff2, "Model name: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        harddisk_get_disk_firmware_version_terminated(type, bus, buff);
        strcpy(buff2, "Firmware version: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        harddisk_get_disk_serial_number_terminated(type, bus, buff);
        strcpy(buff2, "Serial number: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        itoa(harddisk_get_user_addressable_sectors(type, bus), buff, 10);
        strcpy(buff2, "Total number of user addressable sectors: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        itoa(harddisk_get_disk_space(type, bus) / (1024 * 1024), buff, 10);
        strcpy(buff2, "Total number of megabytes: ");
        strcat(buff2, buff);
        strcat(buff2, " MB");
        logger_log_info(buff2);

        if(harddisk_get_is_removable_media_device(type, bus) == true)
        {
            logger_log_info("Removable media: true");
        }
        else
        {
            logger_log_info("Removable media: false");
        }
    }
    else if(state == HARDDISK_ATAPI_PRESENT)
    {
        strcpy(buff2, name);
        strcat(buff2, ": ATAPI device");
        logger_log_info(buff2);

        harddisk_get_disk_model_number_terminated(type, bus, buff);
        strcpy(buff2, "Model name: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        harddisk_get_disk_firmware_version_terminated(type, bus, buff);
        strcpy(buff2, "Firmware version: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        harddisk_get_disk_serial_number_terminated(type, bus, buff);
        strcpy(buff2, "Serial number: ");
        strcat(buff2, buff);
        logger_log_info(buff2);

        if(harddisk_get_is_removable_media_device(type, bus) == true)
        {
            logger_log_info("Removable media: true");
        }
        else
        {
            logger_log_info("Removable media: false");
        }
    }
    else if(state == HARDDISK_NOT_PRESENT)
    {
        strcpy(buff2, name);
        strcat(buff2, ": not detected");
        logger_log_info(buff2);
    }
    else
    {
        strcpy(buff2, name);
        strcat(buff2, ": error or not supported device");
        logger_log_info(buff2);
    }
}

//! Prints hard disks details.
/*! Used during boot to print informations about all hard disks.
 */
void print_harddisks_status()
{
    print_harddisk_details(HARDDISK_ATA_MASTER, HARDDISK_ATA_PRIMARY_BUS, "Primary Master");
    print_harddisk_details(HARDDISK_ATA_SLAVE, HARDDISK_ATA_PRIMARY_BUS, "Primary Slave");
    print_harddisk_details(HARDDISK_ATA_MASTER, HARDDISK_ATA_SECONDARY_BUS, "Secondary Master");
    print_harddisk_details(HARDDISK_ATA_SLAVE, HARDDISK_ATA_SECONDARY_BUS, "Secondary Slave");
}

void startup()
{
    // Must be done before any VGA operation

    volatile uint8_t* scr_ptr = (uint8_t *)(VGA_MODE_03H_BASE_ADDR);
    int i = 0;
    gdt_init();
    paging_init();
    //Don't use VGA before calling VGA init
    vga_init(VGA_MODE_03H);
    logger_log_info("MicrOS is starting...");
    logger_log_ok("BASIC TEXT VGA Driver");
    cpuid_init();

    logger_log_ok("Procesor");
    print_processor_status();

    //Loading Generic VGA Driver
    generic_vga_driver_init();
    logger_log_ok("Loaded DAL, and Generic VGA Driver");

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

    dma_init(0xc0000500);
    logger_log_ok("DMA");

    // NOTE: it doesn't work well, so assume for now that floppy controller is always present
    // if(fdc_is_present())
    {
        fdc_init();
        logger_log_ok("Floppy Disc Controller");
    }
    
    harddisk_init();
    logger_log_ok("Hard Disks");
    print_harddisks_status();

    keyboard_init();
    logger_log_ok("Keyboard");

    partitions_init();
    logger_log_ok("Partitions");

    tss_init();
    logger_log_ok("TSS");


    syscalls_manager_init();
    logger_log_ok("Syscalls manager");

    signals_manager_init();
    logger_log_ok("Signals manager");

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
    //fat_init();
    //logger_log_ok("FAT12");
    
    init_terminal_manager();
    logger_log_ok("Terminal manager");

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

void turn_on_serial_debugging()
{
    serial_init(COM1_PORT, 1200, 8, 1, PARITY_NONE);
    set_debug_traps();
    breakpoint();
}

void v8086_BIOS_timer_interrupt()
{
    timer_interrupt();
    write_dword_to_pointer(v8086_machine->Memory, get_absolute_address(0x40, 0x6c), timer_get_system_clock());
}

int kmain()
{
 static char tmp[] = "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\
ghwu0123456789PCqegwehtyethtgwrtujeywerywryhetwuhetujretyerhjsyv7hsD012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMAFlEMwycr1RdFgYsHmT0L6J5trG57b1Y9LxhTDAUNerIkuAFTnarH31bDep8oHzj2huTSeDGUgOrNB2ABOtcIG4caGdRDhW0SZdlSVYPrtj1rPjbK7yvc78ht\
 mRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeMmRGZiQTpy5FO0JZSdJctNErE4Q8ZOH3GKRhKoEAN1nuv3PCsyv7hsD000000000000000000000000000000000000000000000000000000000000000000000000jgeM";
//static uint32_t tab[8192] = {};


    clear_bss();

    startup();

    logger_log_info("Hello, World!");

    uint32_t value = 0;
    value = 10;
    value = 20;
    value = 6969;

    //startup_music_play();
    logger_log_ok("READY.");
    
    logger_log_ok("Loading shells...");
    
    uint32_t d = 0;
    for (int i = 0; i < 4; i++)
    {
        char args[16];
        itoa(i, args, 10);
        
        uint32_t p = process_manager_create_process("A:/ENV/SHELL.ELF", args, 0, false);
        create_terminal(&d);
    
        uint32_t terminal_number = i;
        const terminal_struct* ts = get_terminals(&terminal_number);
        attach_process_to_terminal(ts[i].terminal_id, process_manager_get_process(p));
    }
    
    vga_clear_screen();
    
    switch_active_terminal(0);
    
    process_manager_run();  

    while (1);
    return 0;
}
