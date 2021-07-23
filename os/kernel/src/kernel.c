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

#ifdef TEST_V8086
#include "v8086/tests/tests.h"
#endif

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
        //fdc_init();
        logger_log_ok("Floppy Disc Controller");
    }
    
    harddisk_init();
    logger_log_ok("Hard Disks");
    print_harddisks_status();

    keyboard_init();
    logger_log_ok("Keyboard");

    //partitions_init();
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
    //uint16_t* vga_ptr = VGA_MODE_03H_BASE_ADDR;
    //vga_ptr[0] = 0xf041;
    //while(1);
    clear_bss();

    startup();
  
    logger_log_info("Hello, World!");

    //startup_music_play();
    logger_log_ok("READY.");
    
    //while (1);
    
    logger_log_ok("Loading shells...");
    
    // create_terminal(&d);
    // create_terminal(&d);
    uint32_t d = 0;
    /*for (int i = 0; i < 4; i++)
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
    keyboard_scan_ascii_pair kb;
    vga_printstring("Press key to continue... (Sending Debug Informations via serial)\n");
    while(!keyboard_get_key_from_buffer(&kb));

    serial_init(COM1_PORT, 115200, 8, 1, PARITY_NONE);

//    v8086_machine = v8086_create_machine();
  //  v8086_set_386_instruction_set(v8086_machine);
    //v8086_machine->regs.x.ax = 0x3;
    //int dupa = v8086_call_int(v8086_machine, 0x10);

    //video_card_set_video_mode(0x3);

    /*uint8_t pre_com[1024];
    memcpy(pre_com, v8086_machine -> Memory, 1024); 
    idt_attach_interrupt_handler(0, v8086_BIOS_timer_interrupt);
    vga_printstring("Starting DOS Program\n");
    int16_t stat = v8086_call_com_program(v8086_machine, "A:/TC.COM");
    vga_printstring("OUT\n");
    uint8_t post_com[1024];
    memcpy(post_com, v8086_machine -> Memory, 1024);
    //memcpy((uint8_t*) 0xc0000000 + 0xb8000, v8086_machine->Memory + 0xb8000, 0xFFFF);
    
    for(int i =0; i<1024; i++){
        uint8_t tmp[100];
        if(post_com[i]!=pre_com[i]){
            vga_printstring("Roznica: ");
            itoa(i, tmp, 10);
            vga_printstring(tmp);
            vga_newline();
        }
    }*/

    v8086_machine = v8086_create_machine();
    v8086_set_386_instruction_set(v8086_machine);
    //idt_attach_interrupt_handler(0, v8086_BIOS_timer_interrupt);

    uint8_t result_text[80];
    uint32_t list_timings[8];
    int16_t statuses[8];
    uint8_t iiii = 0;
    
    v8086_machine->regs.h.ah = 0x0;
    v8086_machine->regs.h.al = 0x03;
    uint32_t start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    uint32_t stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.h.ah = 0x0;
    v8086_machine->regs.h.al = 0x13;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.h.ah = 0x0e;
    v8086_machine->regs.h.al = 0x41;
    v8086_machine->regs.h.bl = 0xf;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.h.ah = 0x0f;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.x.ax = 0x4f00;
    v8086_machine->sregs.es = 0x0;
    v8086_machine->regs.x.di = 0x7e00;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.x.ax = 0x4f01;
    v8086_machine->sregs.es = 0x0;
    v8086_machine->regs.x.di = 0x7e00;
    v8086_machine->regs.x.cx = 0x13;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.x.ax = 0x4f02;
    v8086_machine->regs.x.bx = 0x13;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.x.ax = 0x4f03;
    start = timer_get_system_clock();
    statuses[iiii] = v8086_call_int(v8086_machine, 0x10);
    stop = timer_get_system_clock();
    list_timings[iiii++] = stop - start;

    v8086_machine->regs.h.ah = 0x0;
    v8086_machine->regs.h.al = 0x3;
    v8086_call_int(v8086_machine, 0x10);

    video_card_set_video_mode(0x3);
    vga_clear_screen();
    vga_color c;
    c.color_without_blink.background=VGA_COLOR_BLACK;
    c.color_without_blink.letter=VGA_COLOR_WHITE;
    vga_set_color(0, 0, c);

    for(int ii = 0; ii < 8; ii++){
        //kernel_sprintf(result_text, "FUNKCJA %d: %d", ii, list_timings[ii]);
        vga_printstring("FUNKCJA ");
        itoa(ii, buff, 10);
        vga_printstring(buff);
        vga_printstring(": ");
        itoa(list_timings[ii], buff, 10);
        vga_printstring(buff);
        vga_printstring(" status: ");
        itoa(statuses[ii], buff, 10);
        vga_printstring(buff);
        vga_newline();
        vga_printstring("CS: ");
        itoa(v8086_machine->sregs.cs, buff, 16);
        vga_printstring(buff);
        vga_printstring(" IP: ");
        itoa(v8086_machine->IP.w.ip, buff, 16);
        vga_printstring(buff);
        vga_newline();
    }
    /*vga_printchar('x');
    uint32_t s = timer_get_system_clock();
    while(timer_get_system_clock() - s < 10000);
    vga_printchar('y');*/

    #define FUN_9

    #ifdef FUN_1
        mode13h_set_mode();
        drawLenaIn13H();
    #endif

    #ifdef FUN_2
    v8086* machine = v8086_create_machine();
    v8086_set_386_instruction_set(machine);
    machine->regs.x.ax = 0x13;
    v8086_call_int(machine, 0x10);
    drawLenaIn13H();
    #endif


    #ifdef FUN_3
    char buff[100];
    VBE_initialize();
    setSkipDebugging(true);
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
        for(int i=0; i < svga_info_ptr->number_of_modes; i++)
        //for(int i=0x11b; i < 0x11c; i++)
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
            else{
                physBufforAddress = mode_info.frame_buffor_phys_address;
                /*if(mode_info.mode_height == 200 && mode_info.mode_width == 320 && mode_info.bits_per_pixel == 24)
                {
                    mode_number = svga_info_ptr->mode_array[i];
                        max_width = mode_info.mode_width;
                        max_height = mode_info.mode_height;
                        max_bit_per_pixel = mode_info.bits_per_pixel;
                        break;
                }*/
                if((max_width * max_height) <= ((uint32_t)mode_info.mode_width * (uint32_t)mode_info.mode_height))
                {
                    if(max_bit_per_pixel <= mode_info.bits_per_pixel){
                        mode_number = svga_info_ptr->mode_array[i];
                        max_width = mode_info.mode_width;
                        max_height = mode_info.mode_height;
                        max_bit_per_pixel = mode_info.bits_per_pixel;
                    }
                }
            }
        }
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
        
        /*if(physBufforAddress != 0)
        {
            VBEStatus x = VBE_set_video_mode(0x10f|(1<<14), true);
            VBE_set_current_bank(0);
            paging_map_page(physBufforAddress/0x400000, 12, false);
            uint8_t* color = 12*0x400000 + 0xc00000000;
            drawLenaIn10fH_linear(color);
            paging_unmap_page(12);
        }*/
    }
    else{
        vga_printstring("Unable to get SVGA INFORMATION\n");
    }
    #endif
    
    #ifdef FUN_4
        VBE_initialize();
        VBEStatus x = VBE_set_video_mode(0x10f, true);
        svga_mode_information mode_info;
        VBE_get_vesa_mode_information(&mode_info, 0x10f);
        VBE_display_window_control_set_16bit(0, 0);
        for(int yy = 0; yy < 200; yy++)
        for(int xx = 0; xx < 320; xx++)
                VBE_draw_pixel_8_8_8(mode_info.mode_width, mode_info.mode_height, mode_info.windows_size, mode_info.granularity, xx, yy, 0, 0, 255);

    #endif

    #ifdef FUN_5
        VBE_initialize();
        VBEStatus x = VBE_set_video_mode(0x10f, true);
        VBE_display_window_control_set_16bit(0, 0);
        drawLenaIn10fH();
    #endif
    while (1);
    return 0;
}
