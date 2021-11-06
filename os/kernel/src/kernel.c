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

char *uitoa(unsigned int input, char *buffer, int base)
{
    int idx = 0;
    static const char lowercase_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    // put number to buffer in reverse order
    do
    {
        size_t digit = (input % base);
        buffer[idx++] = lowercase_table[digit];

        input /= base;
    } while (input);

    size_t size = idx;
    idx--;

    // revert string
    for (size_t i = 0; i < size / 2; i++)
    {
        char tmp = buffer[i];
        buffer[i] = buffer[idx];
        buffer[idx] = tmp;

        idx--;
    }

    buffer[size] = '\0';

    return buffer;
}

int kmain()
{
    clear_bss();

    startup();
  
    logger_log_info("Hello, World!");

    //startup_music_play();
    logger_log_ok("READY.");
    //setSkipDebugging(true);
    serial_init(COM1_PORT, 115200, 8, 1, PARITY_NONE);
    
    /*logger_log_ok("Loading shells...");
    
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
    }*/
    
    vga_clear_screen();
    
    //switch_active_terminal(0);
    
    //process_manager_run();  

    char buff[100];
    VBE_initialize();
    //setSkipDebugging(true);
    //VBE_set_video_mode(0x18c, false);
    svga_mode_information mode_info;
    //VBE_get_vesa_mode_information(&mode_info, 0x105);
    VBEStatus status = VBE_check_existance_of_VESA();
    if(status != VBE_OK)
    {
        vga_printstring("Problems with VBE: \n");
        vga_printstring(itoa(status, buff, 10));
        vga_newline();
    }

    status = VBE_DDC_capabilities();
     
    edid edid_block;

    VBE_DDC_read_edid_block(0, &edid_block);

    for(int i = 0; i < 8; i++)
    {
        VESA_std_timing timing = VBE_DDC_decode_std_timing(edid_block.standard_timings_supported[i]);
        char buf[128];
        vga_printstring("RES: ");
        itoa(timing.horizontal_res, buff, 10);
        vga_printstring(buff);
        vga_printstring(" x ");
        itoa(timing.vertical_res, buff, 10);
        vga_printstring(buff);
        vga_printstring(" @ ");
        itoa(timing.refresh_rate, buff, 10);
        vga_printstring(buff);
        vga_printstring(" Hz ASPECT RATIO ");
        switch(timing.aspect_ratio)
        {
            case 0:
                vga_printstring("1:1 or 16:10");
                break;
            case 1:
                vga_printstring("4:3");
                break;
            case 2:
                vga_printstring("5:4");
                break;
            case 3:
                vga_printstring("16:9");
                break;
        }
        vga_newline();
    }

    //Detection code, move somwhere else, there's alot of stuff that will be listed here

    //Established timings I
    if(edid_block.established_supported_timings[0] & 0x80)
        vga_printstring("RES: 720 x 400 @ 70 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x40)
        vga_printstring("RES: 720 x 400 @ 85 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x20)
        vga_printstring("RES: 640 x 480 @ 60 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x10)
        vga_printstring("RES: 640 x 480 @ 67 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x08)
        vga_printstring("RES: 640 x 480 @ 72 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x04)
        vga_printstring("RES: 640 x 480 @ 75 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x02)
        vga_printstring("RES: 800 x 600 @ 56 Hz \n");
    if(edid_block.established_supported_timings[0] & 0x01)
        vga_printstring("RES: 800 x 600 @ 60 Hz \n");

    //Established timings II
    if(edid_block.established_supported_timings[1] & 0x80)
        vga_printstring("RES: 800 x 600 @ 72 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x40)
        vga_printstring("RES: 800 x 600 @ 75 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x20)
        vga_printstring("RES: 1024 x 768 @ 75 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x10)
        vga_printstring("RES: 1024 x 768 @ 87 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x08)
        vga_printstring("RES: 1024 x 768 @ 60 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x04)
        vga_printstring("RES: 1024 x 768 @ 70 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x02)
        vga_printstring("RES: 1024 x 768 @ 75 Hz \n");
    if(edid_block.established_supported_timings[1] & 0x01)
        vga_printstring("RES: 1280 x 1024 @ 75 Hz \n");

    //Manufacturer timings
    if(edid_block.manufacturer_reserved_timing & 0x80)
        vga_printstring("RES: 1152 x 870 @ 75 Hz \n");
    if(edid_block.manufacturer_reserved_timing & 0x40)
        vga_printstring("Unknown, but supported\n");
    if(edid_block.manufacturer_reserved_timing & 0x20)
        vga_printstring("Unknown, but supported\n");
    if(edid_block.manufacturer_reserved_timing & 0x10)
        vga_printstring("Unknown, but supported\n");
    if(edid_block.manufacturer_reserved_timing & 0x08)
        vga_printstring("Unknown, but supported\n");
    if(edid_block.manufacturer_reserved_timing & 0x04)
        vga_printstring("Unknown, but supported\n");
    if(edid_block.manufacturer_reserved_timing & 0x02)
        vga_printstring("Unknown, but supported\n");
    if(edid_block.manufacturer_reserved_timing & 0x01)
        vga_printstring("Unknown, but supported\n");


    VESA_detailed_timing det_timing = {};
    for(int i = 0; i < 4; i++)
    {
        uint16_t det_flag = *(uint16_t*)(&edid_block.detailed_timing_desc[i*18]);
        if(det_flag != 0x0000)
        {
            det_timing = VBE_DDC_read_detailed_timing(&edid_block, 0);
        }
        else
        {
            VESA_monitor_descriptor desc = VBE_DDC_read_monitor_descriptor(&edid_block, i);
            //Add other block reading in this switch.
            switch(desc.data_type)
            {
                case ESTABLISHED_TIMINGS_III:
                    //BYTE 6
                    if(desc.data[0] & 0x80)
                        vga_printstring("640 x 350 @ 85 Hz\n");
                    if(desc.data[0] & 0x40)
                        vga_printstring("640 x 400 @ 85 Hz\n");
                    if(desc.data[0] & 0x20)
                        vga_printstring("720 x 400 @ 85 Hz\n");
                    if(desc.data[0] & 0x10)
                        vga_printstring("640 x 480 @ 85 Hz\n");
                    if(desc.data[0] & 0x08)
                        vga_printstring("848 x 480 @ 60 Hz\n");
                    if(desc.data[0] & 0x04)
                        vga_printstring("800 x 600 @ 85 Hz\n");
                    if(desc.data[0] & 0x02)
                        vga_printstring("1024 x 768 @ 85 Hz\n");
                    if(desc.data[0] & 0x01)
                        vga_printstring("1152 x 864 @ 75 Hz\n");
                    //BYTE 7
                    if(desc.data[1] & 0x80)
                        vga_printstring("1280 x 768 @ 60 Hz (RB)\n");
                    if(desc.data[1] & 0x40)
                        vga_printstring("1280 x 768 @ 60 Hz\n");
                    if(desc.data[1] & 0x20)
                        vga_printstring("1280 x 768 @ 75 Hz\n");
                    if(desc.data[1] & 0x10)
                        vga_printstring("1280 x 768 @ 85 Hz\n");
                    if(desc.data[1] & 0x08)
                        vga_printstring("1280 x 960 @ 60 Hz\n");
                    if(desc.data[1] & 0x04)
                        vga_printstring("1280 x 960 @ 85 Hz\n");
                    if(desc.data[1] & 0x02)
                        vga_printstring("1280 x 1024 @ 60 Hz\n");
                    if(desc.data[1] & 0x01)
                        vga_printstring("1280 x 1024 @ 85 Hz\n");
                    //BYTE 8
                    if(desc.data[2] & 0x80)
                        vga_printstring("1360 x 768 @ 60 Hz\n");
                    if(desc.data[2] & 0x40)
                        vga_printstring("1440 x 900 @ 60 Hz (RB)\n");
                    if(desc.data[2] & 0x20)
                        vga_printstring("1440 x 900 @ 60 Hz\n");
                    if(desc.data[2] & 0x10)
                        vga_printstring("1440 x 900 @ 75 Hz\n");
                    if(desc.data[2] & 0x08)
                        vga_printstring("1440 x 900 @ 85 Hz\n");
                    if(desc.data[2] & 0x04)
                        vga_printstring("1400 x 1050 @ 60 Hz (RB)\n");
                    if(desc.data[2] & 0x02)
                        vga_printstring("1400 x 1050 @ 60 Hz\n");
                    if(desc.data[2] & 0x01)
                        vga_printstring("1400 x 1050 @ 75 Hz\n");
                    //BYTE 9
                    if(desc.data[3] & 0x80)
                        vga_printstring("1400 x 1050 @ 85 Hz\n");
                    if(desc.data[3] & 0x40)
                        vga_printstring("1680 x 1050 @ 60 Hz (RB)\n");
                    if(desc.data[3] & 0x20)
                        vga_printstring("1680 x 1050 @ 60 Hz\n");
                    if(desc.data[3] & 0x10)
                        vga_printstring("1680 x 1050 @ 75 Hz\n");
                    if(desc.data[3] & 0x08)
                        vga_printstring("1680 x 1050 @ 85 Hz\n");
                    if(desc.data[3] & 0x04)
                        vga_printstring("1600 x 1200 @ 60 Hz\n");
                    if(desc.data[3] & 0x02)
                        vga_printstring("1600 x 1200 @ 65 Hz\n");
                    if(desc.data[3] & 0x01)
                        vga_printstring("1600 x 1200 @ 70 Hz\n");
                    //BYTE 10
                    if(desc.data[4] & 0x80)
                        vga_printstring("1600 x 1200 @ 75 Hz\n");
                    if(desc.data[4] & 0x40)
                        vga_printstring("1600 x 1200 @ 85 Hz\n");
                    if(desc.data[4] & 0x20)
                        vga_printstring("1792 x 1344 @ 60 Hz\n");
                    if(desc.data[4] & 0x10)
                        vga_printstring("1792 x 1344 @ 75 Hz\n");
                    if(desc.data[4] & 0x08)
                        vga_printstring("1856 x 1392 @ 60 Hz\n");
                    if(desc.data[4] & 0x04)
                        vga_printstring("1856 x 1392 @ 75 Hz\n");
                    if(desc.data[4] & 0x02)
                        vga_printstring("1920 x 1200 @ 60 Hz (RB)\n");
                    if(desc.data[4] & 0x01)
                        vga_printstring("1920 x 1200 @ 60 Hz\n");
                    //BYTE 11
                    if(desc.data[5] & 0x80)
                        vga_printstring("1920 x 1200 @ 75 Hz\n");
                    if(desc.data[5] & 0x40)
                        vga_printstring("1920 x 1200 @ 85 Hz\n");
                    if(desc.data[5] & 0x20)
                        vga_printstring("1920 x 1440 @ 60 Hz\n");
                    if(desc.data[5] & 0x10)
                        vga_printstring("1920 x 1440 @ 75 Hz\n");
                    break;
                case MONITOR_NAME:
                    for(int i = 0; i < 13; i++)
                    {
                        if(desc.data[i] != '\n')
                            vga_printchar(desc.data[i]);
                        else
                            vga_newline();
                    }
                    break;
                case MONITOR_RANGE_LIMITS:
                    break;
            }
        }
    }
        while (1);

    svga_information* svga_info_ptr;
    status = VBE_get_svga_information(&svga_info_ptr);
    if(status == VBE_OK){
        vga_printstring(svga_info_ptr->signature);
        vga_newline();
        vga_printstring(svga_info_ptr->producent_text);
        vga_newline();
        vga_printstring("VESA VERSION: ");
        itoa(svga_info_ptr->vesa_standard_number, buff, 16);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("VESA NUMBER OF MODES: ");
        itoa(svga_info_ptr->number_of_modes, buff, 10);
        vga_printstring(buff);
        vga_newline();
        uint16_t mode_number = 0;
        uint32_t max_width = 0;
        uint32_t max_height = 0;
        uint16_t max_bit_per_pixel = 0;
        uint32_t physBufforAddress = 0;
        uint16_t mode = 0;
        keyboard_scan_ascii_pair kb;
        for(int i=0; i < svga_info_ptr->number_of_modes; i++)
        //for(int i=0x11b; i < 0x11c; i++)6
        {
            svga_mode_information mode_info;
            status = VBE_get_vesa_mode_information(&mode_info, svga_info_ptr->mode_array[i]);
            if(status != VBE_OK){
                itoa(svga_info_ptr->mode_array[i], buff, 16);
                vga_printstring("Unable to get SVGA MODE INFORMATION: ");
                vga_printstring(buff);
                vga_newline();
                //while(!keyboard_get_key_from_buffer(&kb));
            }
            else
            {
                vga_printstring("MODE ");
                itoa(svga_info_ptr->mode_array[i], buff, 16);
                vga_printstring(buff);
                vga_printstring("h: ");
                itoa(mode_info.mode_width, buff, 10);
                vga_printstring(buff);
                vga_printstring("x");
                itoa(mode_info.mode_height, buff, 10);
                vga_printstring(buff);
                vga_printstring("x");
                itoa(mode_info.bits_per_pixel, buff, 10);
                vga_printstring(buff);
                vga_printstring("b ");
                vga_printstring(mode_info.mode_attributes & 0x80 ? "LINEAR" : "NON-LINEAR");
                vga_printstring(" ");
                vga_printstring(mode_info.mode_attributes & 0x1 ? "SUPPORTED" : "NOT SUPPORTED");
                vga_newline();
                
                while(!keyboard_get_key_from_buffer(&kb));
                if(mode_info.mode_width == 1024 && mode_info.mode_height == 768 && mode_info.bits_per_pixel == 32)
                {
                    mode_number = svga_info_ptr->mode_array[i];
                    max_width = mode_info.mode_width;
                    max_height = mode_info.mode_height;
                    max_bit_per_pixel = mode_info.bits_per_pixel;
                    if(mode_info.frame_buffor_phys_address != 0 && (mode_info.mode_attributes & (0x80 | 0x1)))
                    {
                        mode = mode_number;
                        physBufforAddress = mode_info.frame_buffor_phys_address;
                    }
                }
                // if((max_width * max_height) <= ((uint32_t)mode_info.mode_width * (uint32_t)mode_info.mode_height))
                // { 
                //     if(max_bit_per_pixel <= mode_info.bits_per_pixel)
                //     {
                //         mode_number = svga_info_ptr->mode_array[i];
                //         max_width = mode_info.mode_width;
                //         max_height = mode_info.mode_height;
                //         max_bit_per_pixel = mode_info.bits_per_pixel;
                //         physBufforAddress = mode_info.frame_buffor_phys_address;
                //         mode = mode_number;
                //     }
                // }
            }
        }
        vga_printstring("END_LOOP\n");
        while(!keyboard_get_key_from_buffer(&kb));
        vga_printstring("BEST MODE: ");
        itoa(mode_number, buff, 16);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("MAX WIDTH: ");
        itoa(max_width, buff, 10);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("MAX HEIGHT: ");
        itoa(max_height, buff, 10);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("BITS PER COLOR: ");
        itoa(max_bit_per_pixel, buff, 10);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("PHYSICAL ADDRESS (dec): ");
        itoa(physBufforAddress, buff, 10);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("PHYSICAL ADDRESS (hex): 0x");
        itoa(physBufforAddress, buff, 16);
        vga_printstring(buff);
        vga_newline();
        
        if(physBufforAddress != 0 || mode != 0)
        {
            VBEStatus x = VBE_set_video_mode(mode|(1<<14), true);
            drawLenaIn10fH_linear();
        }
        else
        {
            vga_printstring("MODE NOT SUPPORTED!\n");
        }
    }
    else{
        itoa(status, buff, 16);
        vga_printstring("Unable to get SVGA INFORMATION: ");
        vga_printstring(buff);
        vga_newline();
    }
    
    while (1);
    return 0;
}
