/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * THIS FILE IS MARKED TO BE REFACTORED IN FUTURE. TRY NOT TO ADD NEW THINGS TO PARTS OF OS IN THIS FILE *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "vbe.h"
#include "../v8086/v8086.h"
#include "../v8086/memory_operations.h"
#include <stdbool.h>
#include <string.h>
#include "../memory/heap/heap.h"
#include "../logger/logger.h"
#include "../debug_helpers/library/kernel_stdio.h"
#include "../dal/videocard/videocard.h"

v8086* machine = NULL;
bool initialized = false;
int currentBank = -1;
uint8_t* mem_buff = (uint8_t*)0xc0000000 + 0xA0000;
uint8_t *linear_buffer = 0;
uint32_t linear_buff_size;
uint32_t page_index = 0;
uint32_t page_count = 0;

//EDID Stuff
uint8_t edid_ver = 0;
uint8_t edid_rev = 0;
uint8_t ddc_level = 0;
VESA_detailed_timing native_res;
kvector* monitor_res;

/**
 * Helper function for VBE driver to insert new resolution to monitor supported list.
 *
 * @param h Horizontal resolution
 * @param v Vertical resolution
 * @param rr Refresh rate
 */
static void __VBE_add_res_to_monitor_list(uint16_t h, uint16_t v, uint16_t rr)
{
    VBE_resolution* res = heap_kernel_alloc(sizeof(VBE_resolution), 0);
    res->horizontal = h;
    res->vertical = v;
    res->refresh_rate = rr;
    res->pixel_clock = -1;
    kvector_add(monitor_res, res);
}


void VBE_init_driver()
{

}

static uint8_t __VBE_find_mode_in_monitor_list(uint16_t h, uint16_t v)
{
    for(uint32_t i = 0; i < monitor_res->count; i++)
    {
        VBE_resolution* res_ptr = ((VBE_resolution*)monitor_res->data)+i;
        if( res_ptr->horizontal == h &&
            res_ptr->vertical == v)
            return 1;
    }
    return 0;
}

void VBE_initialize()
{
    char buff[128];
    //Create and initialize v8086
    if(!initialized)
    {
        machine = v8086_create_machine();
        if(machine == NULL) return;
        v8086_set_386_instruction_set(machine);
        initialized = true;
    }

    //Check if we got working v8086
    if(!initialized)
    {
        logger_log_error("Couldn't initialize v8086!");
        return;
    }

    //Check if VBE is supported.
    VBEStatus status = VBE_check_existance_of_VESA();
    if(status != VBE_OK)
    {
        kernel_sprintf(buff, "VBE Support unavailable, VBE Error code: %d.", status);
        logger_log_error(buff);
        return;
    }

    //Check if we support DDC
    status = VBE_DDC_capabilities();
    if(status != VBE_OK)
    {
        logger_log_error("DDC support not found. Cannot read supported resolutions.");
        return;
    }

    edid edid_block;

    //Read first EDID block (block num 0)
    status = VBE_DDC_read_edid_block(0, &edid_block);
    if(status != VBE_OK)
    {
        logger_log_error("EDID block read failed!");
        return;
    }

    //Init monitor resolution vector
    monitor_res = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(monitor_res);

    //Decode standard timings and insert them to monitor suported resolutions
    //TODO BUG FIX: HDMI seems to include weird resolutions, possibly needs to be checked with newer versions of DDC and EDID (EDDC and EEDID).
    for(int i = 0; i < 8; i++)
    {
        VESA_std_timing timing = VBE_DDC_decode_std_timing(edid_block.standard_timings_supported[i]);
        __VBE_add_res_to_monitor_list(timing.horizontal_res, timing.vertical_res, timing.refresh_rate);
    }

    //This part is going to be long and very repeatetive
    //Blame VESA for creating such weird way to list timings.
    //If you can come up with better way to insert these.
    //PLEASE DO CHANGE THIS.

    //Established timings I
    if(edid_block.established_supported_timings[0] & 0x80)
        __VBE_add_res_to_monitor_list(720, 400, 70);
    if(edid_block.established_supported_timings[0] & 0x40)
        __VBE_add_res_to_monitor_list(720, 400, 85);
    if(edid_block.established_supported_timings[0] & 0x20)
        __VBE_add_res_to_monitor_list(640, 480, 60);
    if(edid_block.established_supported_timings[0] & 0x10)
        __VBE_add_res_to_monitor_list(640, 480, 67);
    if(edid_block.established_supported_timings[0] & 0x08)
        __VBE_add_res_to_monitor_list(640, 480, 72);
    if(edid_block.established_supported_timings[0] & 0x04)
        __VBE_add_res_to_monitor_list(640, 480, 75);
    if(edid_block.established_supported_timings[0] & 0x02)
        __VBE_add_res_to_monitor_list(800, 600, 56);
    if(edid_block.established_supported_timings[0] & 0x01)
        __VBE_add_res_to_monitor_list(800, 600, 60);

    //Established timings II
    if(edid_block.established_supported_timings[1] & 0x80)
        __VBE_add_res_to_monitor_list(800, 600, 72);
    if(edid_block.established_supported_timings[1] & 0x40)
        __VBE_add_res_to_monitor_list(800, 600, 75);
    if(edid_block.established_supported_timings[1] & 0x20)
        __VBE_add_res_to_monitor_list(832, 624, 75);
    if(edid_block.established_supported_timings[1] & 0x10)
        __VBE_add_res_to_monitor_list(1024, 768, 87);
    if(edid_block.established_supported_timings[1] & 0x08)
        __VBE_add_res_to_monitor_list(1024, 768, 60);
    if(edid_block.established_supported_timings[1] & 0x04)
        __VBE_add_res_to_monitor_list(1024, 768, 70);
    if(edid_block.established_supported_timings[1] & 0x02)
        __VBE_add_res_to_monitor_list(1024, 768, 75);
    if(edid_block.established_supported_timings[1] & 0x01)
        __VBE_add_res_to_monitor_list(1280, 1024, 75);

    //Manufacturer timings
    if(edid_block.manufacturer_reserved_timing & 0x80)
        __VBE_add_res_to_monitor_list(1152, 870, 75);

    //Code below ~should~ be fine now.

    //Read display descritors, EDID prior to 1.3 defined 4 descriptors, from 1.3 first block is native resolution/timing
    for(int i = 0; i < 4; i++)
    {
        uint16_t det_flag = *(uint16_t*)(&edid_block.detailed_timing_desc[i*18]);
        if(det_flag != 0x0000)
        {
            native_res = VBE_DDC_read_detailed_timing(&edid_block, 0);
        }
        else
        {
            VESA_monitor_descriptor desc = VBE_DDC_read_monitor_descriptor(&edid_block, i);
            //Add other block reading in this switch.
            switch(desc.data_type)
            {
                //Please do not remove RB comments, this is to note which resolution has reduced blanking for future.
                case ESTABLISHED_TIMINGS_III:
                {
                    //BYTE 6
                    if(desc.data[0] & 0x80)
                        __VBE_add_res_to_monitor_list(640, 350, 85);
                    if(desc.data[0] & 0x40)
                        __VBE_add_res_to_monitor_list(640, 400, 85);
                    if(desc.data[0] & 0x20)
                        __VBE_add_res_to_monitor_list(720, 400, 85);
                    if(desc.data[0] & 0x10)
                        __VBE_add_res_to_monitor_list(640, 480, 85);
                    if(desc.data[0] & 0x08)
                        __VBE_add_res_to_monitor_list(848, 480, 60);
                    if(desc.data[0] & 0x04)
                        __VBE_add_res_to_monitor_list(800, 600, 85);
                    if(desc.data[0] & 0x02)
                        __VBE_add_res_to_monitor_list(1024, 768, 85);
                    if(desc.data[0] & 0x01)
                        __VBE_add_res_to_monitor_list(1152, 864, 75);

                    //BYTE 7
                    if(desc.data[1] & 0x80)
                        __VBE_add_res_to_monitor_list(1280, 768, 60); //RB
                    if(desc.data[1] & 0x40)
                        __VBE_add_res_to_monitor_list(1280, 768, 60);
                    if(desc.data[1] & 0x20)
                        __VBE_add_res_to_monitor_list(1280, 768, 75);
                    if(desc.data[1] & 0x10)
                        __VBE_add_res_to_monitor_list(1280, 768, 85);
                    if(desc.data[1] & 0x08)
                        __VBE_add_res_to_monitor_list(1280, 960, 60);
                    if(desc.data[1] & 0x04)
                        __VBE_add_res_to_monitor_list(1280, 960, 85);
                    if(desc.data[1] & 0x02)
                        __VBE_add_res_to_monitor_list(1280, 1024, 60);
                    if(desc.data[1] & 0x01)
                        __VBE_add_res_to_monitor_list(1280, 1024, 85);

                    //BYTE 8
                    if(desc.data[2] & 0x80)
                        __VBE_add_res_to_monitor_list(1360, 768, 60);
                    if(desc.data[2] & 0x40)
                        __VBE_add_res_to_monitor_list(1440, 900, 60); //RB
                    if(desc.data[2] & 0x20)
                        __VBE_add_res_to_monitor_list(1440, 900, 60);
                    if(desc.data[2] & 0x10)
                        __VBE_add_res_to_monitor_list(1440, 900, 75);
                    if(desc.data[2] & 0x08)
                        __VBE_add_res_to_monitor_list(1440, 900, 85);
                    if(desc.data[2] & 0x04)
                        __VBE_add_res_to_monitor_list(1440, 1050, 60); //RB
                    if(desc.data[2] & 0x02)
                        __VBE_add_res_to_monitor_list(1440, 1050, 60);
                    if(desc.data[2] & 0x01)
                        __VBE_add_res_to_monitor_list(1440, 1050, 75);

                    //BYTE 9
                    if(desc.data[3] & 0x80)
                        __VBE_add_res_to_monitor_list(1440, 1050, 85);
                    if(desc.data[3] & 0x40)
                        __VBE_add_res_to_monitor_list(1680, 1050, 60); //RB
                    if(desc.data[3] & 0x20)
                        __VBE_add_res_to_monitor_list(1680, 1050, 60);
                    if(desc.data[3] & 0x10)
                        __VBE_add_res_to_monitor_list(1680, 1050, 75);
                    if(desc.data[3] & 0x08)
                        __VBE_add_res_to_monitor_list(1680, 1050, 85);
                    if(desc.data[3] & 0x04)
                        __VBE_add_res_to_monitor_list(1600, 1200, 60);
                    if(desc.data[3] & 0x02)
                        __VBE_add_res_to_monitor_list(1600, 1200, 65);
                    if(desc.data[3] & 0x01)
                        __VBE_add_res_to_monitor_list(1600, 1200, 70);

                    //BYTE 10
                    if(desc.data[4] & 0x80)
                        __VBE_add_res_to_monitor_list(1600, 1200, 75);
                    if(desc.data[4] & 0x40)
                        __VBE_add_res_to_monitor_list(1600, 1200, 85);
                    if(desc.data[4] & 0x20)
                        __VBE_add_res_to_monitor_list(1792, 1344, 60);
                    if(desc.data[4] & 0x10)
                        __VBE_add_res_to_monitor_list(1792, 1344, 75);
                    if(desc.data[4] & 0x08)
                        __VBE_add_res_to_monitor_list(1856, 1392, 60);
                    if(desc.data[4] & 0x04)
                        __VBE_add_res_to_monitor_list(1856, 1392, 75);
                    if(desc.data[4] & 0x02)
                        __VBE_add_res_to_monitor_list(1920, 1200, 60); //RB
                    if(desc.data[4] & 0x01)
                        __VBE_add_res_to_monitor_list(1920, 1200, 60); 

                    //BYTE 11
                    if(desc.data[5] & 0x80)
                        __VBE_add_res_to_monitor_list(1920, 1200, 75); 
                    if(desc.data[5] & 0x40)
                        __VBE_add_res_to_monitor_list(1920, 1200, 85); 
                    if(desc.data[5] & 0x20)
                        __VBE_add_res_to_monitor_list(1920, 1440, 60); 
                    if(desc.data[5] & 0x10)
                        __VBE_add_res_to_monitor_list(1920, 1440, 75); 
                    break;
                }
                case MONITOR_NAME:
                    // for(int i = 0; i < 13; i++)
                    // {
                    //     if(desc.data[i] != '\n')
                    //         vga_printchar(desc.data[i]);
                    //     else
                    //         vga_newline();
                    // }
                    // vga_newline();
                    logger_log_warning("Monitor name ignored for now.");
                    break;

                case MONITOR_RANGE_LIMITS:
                    logger_log_warning("Monitor range limits not yet supported!");
                    break;

                case STANDARD_TIMINGS:
                    for(int i = 0; i < 6; i++)
                    {
                        VESA_std_timing timing = VBE_DDC_decode_std_timing(*(uint16_t*)&desc.data[i*sizeof(uint16_t)]);
                        __VBE_add_res_to_monitor_list(timing.horizontal_res, timing.vertical_res, timing.refresh_rate);
                    }
                    break;

                default:
                    kernel_sprintf(buff, "Unknown display descriptor: %d", desc.data_type);
                    logger_log_warning(buff);
                    break;
            }
        }
    }

    kernel_sprintf(buff, "EDID EXTENSIONS: %d", edid_block.extension_num);
    logger_log_info(buff);

    // read extensions blocks
    // for now these are pretty much discarded, PART 2 will implement them properly.
    // if your display has any extensions available (extension_num is 1 or greater)
    // you can uncomment code inside for loop to list them.
    // Please note all IDs that will be listed and share them with us.
    // This also can take some time... DDC isn't very fast. ¯\_(ツ)_/¯
    logger_log_warning("EDID Extensions are ignored in this driver version.");
    //uint8_t* edid_extension = heap_kernel_alloc(128, 0);
    // if(edid_block.extension_num > 0)
    // {
    //     for(int i = 1; i <= edid_block.extension_num; i++)
    //     {
    //         VBE_DDC_read_edid_block(i, edid_extension);
    //         //uint8_t block_id = *edid_extension;
    //         //itoa(block_id, buff, 16);
    //         // vga_printstring("EXTENSION ID: ");
    //         // vga_printstring(buff);
    //         // vga_newline();
    //         // if(block_id == 0x10)
    //         // {
    //         //     vga_printstring("VTB EXTENSION FOUND");
    //         //     vga_newline();
    //         // }
    //     }
    // }

    //Now read GPU stuff
    svga_information* svga_info_ptr;
    status = VBE_get_svga_information(&svga_info_ptr);
    if(status != VBE_OK)
    {   
        kernel_sprintf(buff, "VBE Controller Information read failed, VBE Error code: %d.", status);
        logger_log_error(buff);
        return;  
    }

    //OK, VBE controller exists so we can now setup driver initialize structure
    //Each pointer has to point to implementation in this driver.
    //Since DAL is still not really used for abstraction yet, we just need to make sure functions work fine.
    //If function is not implemented it should return -1
    driver_init_struct* s = heap_kernel_alloc(sizeof(driver_init_struct), 0);

    s->set_video_mode = __vbe_set_video_mode;

    s->turn_on_buffer = __vbe_turn_on_buffer;
    s->turn_off_buffer = __vbe_turn_off_buffer;
    s->is_buffer_on = __vbe_is_buffer_on;
    s->swap_buffers = __vbe_swap_buffers;

    s->print_char = __vbe_print_char;
    s->print_char_color = __vbe_print_char_color;
    s->print_string = __vbe_print_string;
    s->print_string_color = __vbe_print_string_color;
    s->set_char = __vbe_set_char;
    s->get_char = __vbe_get_char;
    s->set_color = __vbe_set_color;
    s->get_color = __vbe_get_color;
    s->set_char_and_color = __vbe_set_char_and_color;
    s->get_char_and_color = __vbe_get_char_and_color;
    s->set_cursor_pos = __vbe_set_cursor_pos;
    s->get_cursor_pos = __vbe_get_cursor_pos;
    s->turn_cursor_on = __vbe_turn_cursor_on;
    s->turn_cursor_off = __vbe_turn_cursor_off;

    s->print_char_external_buffer = __vbe_print_char_external_buffer;
    s->print_char_color_external_buffer = __vbe_print_char_color_external_buffer;
    s->print_string_external_buffer = __vbe_print_string_external_buffer;
    s->print_string_color_external_buffer = __vbe_print_string_color_external_buffer;
    s->set_char_external_buffer = __vbe_set_char_external_buffer;
    s->get_char_external_buffer = __vbe_get_char_external_buffer;
    s->set_color_external_buffer = __vbe_set_color_external_buffer;
    s->get_color_external_buffer = __vbe_get_color_external_buffer;
    s->set_char_and_color_external_buffer = __vbe_set_char_and_color_external_buffer;
    s->get_char_and_color_external_buffer = __vbe_get_char_and_color_external_buffer;

    s->draw_pixel = __vbe_draw_pixel;
    s->draw_line = __vbe_draw_line;
    s->draw_circle = __vbe_draw_circle;
    s->draw_rectangle = __vbe_draw_rectangle;
    s->clear_screen = __vbe_clear_screen;

    s->draw_pixel_external_buffer = __vbe_draw_pixel_external_buffer;
    s->draw_line_external_buffer = __vbe_draw_line_external_buffer;
    s->draw_circle_external_buffer = __vbe_draw_circle_external_buffer;
    s->draw_rectangle_external_buffer = __vbe_draw_rectangle_external_buffer;
    s->clear_screen_external_buffer = __vbe_clear_screen_external_buffer;

    s->swap_external_buffer = __vbe_swap_external_buffer;
    s->create_external_buffer = __vbe_create_external_buffer;
    s->destroy_external_buffer = __vbe_destroy_external_buffer;

    for(size_t i = 0; i < svga_info_ptr->number_of_modes; i++)
    {
        svga_mode_information mode_info;
        //Get VESA mode information
        status = VBE_get_vesa_mode_information(&mode_info, svga_info_ptr->mode_array[i]);
        if(status == VBE_OK && (mode_info.mode_attributes & 0x01) && mode_info.frame_buffor_phys_address != 0)
        {
            video_mode* mode_ptr = (video_mode*)heap_kernel_alloc(sizeof(video_mode), 0);
            //DEBUG
            //char buff[128];
            //kernel_sprintf(buff, "MODE %04X LFB PHYSICAL ADDRESS: %08X\n", svga_info_ptr->mode_array[i], mode_info.frame_buffor_phys_address);
            //vga_printstring(buff);
            //keyboard_scan_ascii_pair kb;
            //while(!keyboard_get_key_from_buffer(&kb));
            mode_ptr->id = svga_info_ptr->mode_array[i];
            mode_ptr->width = mode_info.mode_width;
            mode_ptr->height = mode_info.mode_height;
            switch(mode_info.bits_per_pixel)
            {
                case 8:
                    mode_ptr->colors = COLOR_8B;
                    break;
                case 16:
                    mode_ptr->colors = COLOR_16B;
                    break;
                case 24:
                    mode_ptr->colors = COLOR_24B;
                    break;
                case 32:
                    mode_ptr->colors = COLOR_32B;
                    break;
            }
            mode_ptr->monochrome = 0;
            mode_ptr->planar = !(mode_info.mode_attributes & 0x80);
            mode_ptr->text = !(mode_info.mode_attributes & 0x10);
            mode_ptr->bpp = mode_info.bits_per_pixel;
            mode_ptr->linear_buffer_size = mode_info.mode_height*mode_info.mode_width*(mode_info.bits_per_pixel/8);
            //Check if current monitor supports this mode.
            //Kinda simple check here, but it is just to prevent resolutions that are completely not correct size.
            mode_ptr->monitor_supported = __VBE_find_mode_in_monitor_list(mode_ptr->width, mode_ptr->height);
            mode_ptr->dis_ptr = s;
            video_card_add_mode(mode_ptr);
        }
    }
}

void VBE_close()
{
    if(initialized)
    {
        v8086_destroy_machine(machine);
        initialized = false;
    }
}

bool VBE_is_initialized()
{
    return initialized;
}

VBEStatus VBE_check_existance_of_VESA()
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f03;
    v8086_call_int(machine, 0x10);
    if(machine->regs.x.ax != 0x004f) return VBE_NOT_EXIST;
    return VBE_OK;
}

VBEStatus VBE_get_svga_information(svga_information** information_struct_ptr){
    if(!initialized) return VBE_NO_INITAILIZED;
    svga_information* information_struct = heap_kernel_alloc(sizeof(svga_information), 0);
    if(information_struct == NULL) return VBE_INTERNAL_ERROR;
    machine->regs.x.ax = 0x4f00;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x7E00;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *(uint32_t*)(information_struct->signature) = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E00));
    information_struct->signature[5] = '\0';
    information_struct->vesa_standard_number = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E04));
    uint16_t off = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E06));
    uint16_t seg = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E08));
    uint16_t len = strlen((char*)(machine->Memory + get_absolute_address(seg, off)));
    information_struct->producent_text = heap_kernel_alloc(len + 1, 0);
    strcpy(information_struct->producent_text, machine->Memory + get_absolute_address(seg, off));
    information_struct->additional_info = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E0A));
    off = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E0E));
    seg = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E10));
    for(len = 0; read_word_from_pointer(machine->Memory, get_absolute_address(seg, off + 2*len)) != 0xffff; ++len);
    information_struct->number_of_modes = len;
    information_struct->mode_array = heap_kernel_alloc(2 * len, 0);
    memcpy(information_struct->mode_array, machine->Memory + get_absolute_address(seg, off), len * 2);
    *information_struct_ptr = information_struct;
    return VBE_OK;
}

VBEStatus VBE_get_vesa_mode_information(svga_mode_information* infromation_struct, uint16_t mode_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f01;
    machine->regs.x.cx = mode_number;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x7E00;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    infromation_struct->mode_attributes = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E00));
    infromation_struct->window_a_attributes = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E02));
    infromation_struct->window_b_attributes = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E03));
    infromation_struct->granularity = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E04));
    infromation_struct->windows_size = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E06));
    infromation_struct->segment_of_window_a = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E08));
    infromation_struct->segment_of_window_b = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E0A));
    infromation_struct->far_pointer_to_bank_swap_function = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E0C));
    infromation_struct->logical_line_length = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E10));
    infromation_struct->mode_width = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E12));
    infromation_struct->mode_height = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E14));
    infromation_struct->char_matrix_width = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E16));
    infromation_struct->char_matrix_height = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E17));
    infromation_struct->plane_count = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E18));
    infromation_struct->bits_per_pixel = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E19));
    infromation_struct->bank_count = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E1A));
    infromation_struct->memory_organization = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E1B));
    infromation_struct->bank_size = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E1C));
    infromation_struct->page_count = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E1D));
    infromation_struct->reserved = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E1E));
    infromation_struct->mask_size_red = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E1F));
    infromation_struct->field_position_red = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E20));
    infromation_struct->mask_size_green = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E21));
    infromation_struct->field_position_green = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E22));
    infromation_struct->mask_size_blue = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E23));
    infromation_struct->field_position_blue = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E24));
    infromation_struct->mask_size_direct_color = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E25));
    infromation_struct->field_position_direct_color = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E26));
    infromation_struct->direct_color_mode_info = read_byte_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E27));

    infromation_struct->frame_buffor_phys_address = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E28));
    infromation_struct->reserved1 = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E2C));
    infromation_struct->reserved2 = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E30));
    return VBE_OK;
}

VBEStatus VBE_DDC_capabilities()
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f15;
    machine->regs.h.bl = 0x00;
    machine->regs.x.cx = 0x0000;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x0000;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;

    ddc_level = machine->regs.h.bl;
    return VBE_OK;
}

VBEStatus VBE_DDC_read_edid_block(uint16_t block_number, edid* block_ptr)
{
    if(ddc_level & DDC1_SUPPORT || ddc_level & DDC2_SUPPORT)
    {
        if(!initialized) return VBE_NO_INITAILIZED;
        machine->regs.x.ax = 0x4f15;
        machine->regs.h.bl = 0x01;
        machine->regs.x.cx = 0x0000;
        machine->regs.x.dx = block_number;
        machine->sregs.es = 0x0000;
        machine->regs.x.di = 0x7E00;
        int16_t status = v8086_call_int(machine, 0x10);
        if(status != 0x10) return VBE_INTERNAL_ERROR;
        if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
        if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;

        memcpy(block_ptr, machine->Memory + get_absolute_address(0x0000, 0x7E00), sizeof(edid));

        edid_ver = block_ptr->edid_version;
        edid_rev = block_ptr->edid_revision;

        return VBE_OK;
    }
    return VBE_FUNCTION_FAILURE;
}


VESA_std_timing VBE_DDC_decode_std_timing(uint16_t timing)
{
    VESA_std_timing _timing;
    _timing.horizontal_res = ((timing & 0xFF) + 31) * 8;
    _timing.aspect_ratio = (((timing >> 8) & 0xC0) >> 6);
    switch(_timing.aspect_ratio )
    {
        //1:1 or 16:10, Consult EEDID specs from VESA
        case 0:
            if(edid_ver == 1 && edid_rev < 3)
                _timing.vertical_res = _timing.horizontal_res;
            else
                _timing.vertical_res = (_timing.horizontal_res/16)*10;
            break;
        //4:3
        case 1:
            _timing.vertical_res = (_timing.horizontal_res/4)*3;
            break;
        //5:4
        case 2:
            _timing.vertical_res = (_timing.horizontal_res/5)*4;
            break;
        //16:9
        case 3:
            _timing.vertical_res = (_timing.horizontal_res/16)*9;
            break;
    }
    _timing.refresh_rate = ((timing >> 8) & 0x3F) + 60;
    return _timing;
}

//Yeah I know this looks weird, ask VESA why...
VESA_detailed_timing VBE_DDC_read_detailed_timing(edid* block_ptr, uint8_t detailed_block_num)
{
    uint8_t* byte_ptr = &block_ptr->detailed_timing_desc[detailed_block_num*18];
    VESA_detailed_timing dt = {};
    dt.pixel_clock               =  *(uint16_t*)(byte_ptr);
    dt.horizontal_active        |=  0xFF & *(byte_ptr+0x02);
    dt.horizontal_blanking      |=  0xFF & *(byte_ptr+0x03);
    dt.horizontal_active        |=  ((0xF0 & *(byte_ptr+0x04)) >> 4) << 8;
    dt.horizontal_blanking      |=  (0x0F & *(byte_ptr+0x04)) << 8;
    dt.vertical_active          |=  0xFF & *(byte_ptr+0x05);
    dt.vertical_blanking        |=  0xFF & *(byte_ptr+0x06);
    dt.vertical_active          |=  ((0xF0 & *(byte_ptr+0x07)) >> 4) << 8;
    dt.vertical_blanking        |=  (0x0F & *(byte_ptr+0x07)) << 8;
    dt.hsync_offset             |=  0xFF & *(byte_ptr+0x08);
    dt.hsync_pulse_width        |=  0xFF & *(byte_ptr+0x09);
    dt.vsync_offset             |=  (0xF0 & *(byte_ptr+0x0A)) >> 4;
    dt.vsync_pulse_width        |=  (0x0F & *(byte_ptr+0x0A));
    dt.hsync_offset             |=  ((0xC0 & *(byte_ptr+0x0B)) >> 6) << 8;
    dt.hsync_pulse_width        |=  ((0x30 & *(byte_ptr+0x0B)) >> 4) << 8;
    dt.vsync_offset             |=  ((0x0C & *(byte_ptr+0x0B)) >> 2) << 8;
    dt.vsync_pulse_width        |=  (0x03 & *(byte_ptr+0x0B)) << 8;
    dt.horizontal_image_size    |=  0xFF & *(byte_ptr+0xC);
    dt.vertical_image_size      |=  0xFF & *(byte_ptr+0xD);
    dt.horizontal_image_size    |=  ((0xF0 & *(byte_ptr+0x0E)) >> 4) << 8;
    dt.vertical_image_size      |=  (0x0F & *(byte_ptr+0x0E)) << 8;
    dt.horizontal_border         =  0xFF & *(byte_ptr+0x0F);
    dt.vertical_border           =  0xFF & *(byte_ptr+0x10);
    dt.flags                     =  0xFF & *(byte_ptr+0x11);
    return dt;
}

VESA_monitor_descriptor VBE_DDC_read_monitor_descriptor(edid* block_ptr, uint8_t block_num)
{
    VESA_monitor_descriptor md = {};
    memcpy(&md, &block_ptr->detailed_timing_desc[block_num*18], 18);
    return md;
}

VBE_resolution VBE_create_resolution_def(uint16_t horizontal, uint16_t vertical, uint16_t refresh_rate)
{
    VBE_resolution res = {};
    res.horizontal = horizontal;
    res.vertical = vertical;
    res.refresh_rate = refresh_rate;
    res.pixel_clock = -1;
    //TODO ADD PROPER TIMING HERE
    return res;
}

uint16_t VBE_get_word(uint32_t seg, uint32_t offset)
{
    return read_word_from_pointer(machine->Memory, get_absolute_address(seg, offset));
}

uint32_t VBE_get_dword(uint32_t seg, uint32_t offset)
{
    return read_dword_from_pointer(machine->Memory, get_absolute_address(seg, offset));
}

VBEStatus VBE_destroy_svga_information(svga_information* svga_information_ptr)
{
    heap_kernel_dealloc(svga_information_ptr->mode_array);
    heap_kernel_dealloc(svga_information_ptr->producent_text);
    heap_kernel_dealloc(svga_information_ptr);
    return VBE_OK;
}

//TODO This might be broken. Examine paging and mapping memory.
VBEStatus VBE_set_video_mode(uint16_t mode_number, bool clear_screen)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f02;
    machine->regs.x.bx = mode_number & 0x7FFF;
    if(clear_screen) machine->regs.x.bx = machine->regs.x.bx | 0x8000;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    currentBank = -1;

    svga_mode_information mode_info;
    status = VBE_get_vesa_mode_information(&mode_info, mode_number);
    if(status == VBE_OK)
    {
        if(mode_info.frame_buffor_phys_address != 0)
        {
            if(page_index != 0)
            {
                for(unsigned int i = 0; i < page_count+1; i++)
                {
                    paging_unmap_page(page_index+i);
                }
                page_index = 0;
                page_count = 0;
                linear_buffer = 0;
            }
            
            linear_buff_size = mode_info.mode_height*mode_info.mode_width*(mode_info.bits_per_pixel/8);
            page_count = linear_buff_size / 0x400000;

            
            uint32_t tempIndex = 1;
            uint32_t prevIndex = tempIndex;
            int i = 0;
            while(i < page_count+1)
            {
                tempIndex = paging_get_first_free_page_index(prevIndex);
                if( i == 0 || tempIndex == prevIndex)
                {
                    i++;
                }
                else
                {
                    i = 0;
                }
                prevIndex = tempIndex+1;
            }
            page_index = tempIndex - page_count;

            for(int i = 0; i < page_count+1; i++)
            {
                paging_map_page((mode_info.frame_buffor_phys_address+ i * 0x400000) / 0x400000, page_index + i, false);
            }

            linear_buffer = (uint8_t*)(page_index * 0x400000)+ 0xc00000000;
        }
        else
        {
            if(page_index != 0)
            {
                for(int i = 0; i < page_count+1; i++)
                {
                    paging_unmap_page(page_index+i);
                }
                page_index = 0;
                page_count = 0;
                linear_buffer = 0;
            }
        }
    }
    return VBE_OK;
}

VBEStatus VBE_get_current_video_mode(uint16_t* mode_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f03;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *mode_number = machine->regs.x.bx;
    return VBE_OK;
}

VBEStatus VBE_set_current_bank(uint32_t bank_number)
{
    if(bank_number != currentBank)
    {
        machine->regs.x.ax = 0x4f05;
        machine->regs.x.bx = 0;
        machine->regs.x.dx = bank_number;
        int16_t status = v8086_call_int(machine, 0x10);
        if(status != 0x10) return VBE_INTERNAL_ERROR;
        if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
        if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
        /*machine->regs.x.ax = 0x4f05;
        machine->regs.x.bx = 0;
        machine->regs.x.dx = bank_number;
        status = v8086_call_int(machine, 0x10);
        if(status != 0x10) return VBE_INTERNAL_ERROR;
        if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
        if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;*/
        currentBank = bank_number;
        return VBE_OK;
    }
    return VBE_OK;
}



VBEStatus VBE_return_save_restore_state_buffer_size(uint16_t requested_states, uint16_t* buffer_block_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f04;
    machine->regs.h.dl = 0x00;
    machine->regs.x.cx = requested_states;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *buffer_block_number = machine->regs.x.bx;
    return VBE_OK;
}

VBEStatus VBE_save_state(uint16_t requested_states, uint16_t buffer_pointer)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f04;
    machine->regs.h.dl = 0x01;
    machine->regs.x.cx = requested_states;
    machine->sregs.es = buffer_pointer;
    machine->regs.x.bx = 0x00;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_restore_state(uint16_t requested_states, uint16_t buffer_pointer)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f04;
    machine->regs.h.dl = 0x02;
    machine->regs.x.cx = requested_states;
    machine->sregs.es = buffer_pointer;
    machine->regs.x.bx = 0x00;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_display_window_control_set_16bit(uint8_t window_number, uint8_t window_mem_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f05;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = window_number;
    machine->regs.x.dx = window_mem_number;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_display_window_control_get_16bit(uint8_t window_number, uint8_t* window_mem_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f05;
    machine->regs.h.bh = 0x01;
    machine->regs.h.bl = window_number;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *window_mem_number = machine->regs.x.dx;
    return VBE_OK;
}

VBEStatus VBE_display_window_control_set_32bit(uint8_t window_number, uint8_t window_mem_number, uint16_t memory_selector)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = window_number;
    machine->regs.x.dx = window_mem_number;
    machine->sregs.es = memory_selector;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_get_logical_scan_line_length(uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f06;
    machine->regs.h.bl = 0x01;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *bytes_per_line = machine->regs.x.bx;
    *actual_pixel_in_line = machine->regs.x.cx;
    *maximum_scan_lines_number = machine->regs.x.dx;
    return VBE_OK;
}

VBEStatus VBE_get_maximum_logical_scan_line_length(uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f06;
    machine->regs.h.bl = 0x03;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *bytes_per_line = machine->regs.x.bx;
    *actual_pixel_in_line = machine->regs.x.cx;
    *maximum_scan_lines_number = machine->regs.x.dx;
    return VBE_OK;
}

VBEStatus VBE_set_logical_scan_line_length_in_pixels(uint16_t length, uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f06;
    machine->regs.h.bl = 0x00;
    machine->regs.x.cx = length;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *bytes_per_line = machine->regs.x.bx;
    *actual_pixel_in_line = machine->regs.x.cx;
    *maximum_scan_lines_number = machine->regs.x.dx;
    return VBE_OK;
}

VBEStatus VBE_set_logical_scan_line_length_in_bytes(uint16_t length, uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f06;
    machine->regs.h.bl = 0x02;
    machine->regs.x.cx = length;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *bytes_per_line = machine->regs.x.bx;
    *actual_pixel_in_line = machine->regs.x.cx;
    *maximum_scan_lines_number = machine->regs.x.dx;
    return VBE_OK;
}

VBEStatus VBE_set_display_start_16bit(bool during_vertical_retrace, uint16_t first_line, uint16_t first_pixel_in_line)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = during_vertical_retrace?0x80:0x00;
    machine->regs.x.cx = first_pixel_in_line;
    machine->regs.x.dx = first_line;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_get_display_start_16bit(uint16_t* first_line, uint16_t* first_pixel_in_line)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = 0x01;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *first_pixel_in_line = machine->regs.x.cx;
    *first_line = machine->regs.x.dx;
    return VBE_OK;
}

VBEStatus VBE_schedule_display_start_16bit(bool during_vertical_retrace, uint32_t display_start_address_byte)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = during_vertical_retrace?0x082:0x02;
    machine->regs.d.ecx = display_start_address_byte;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_schedule_stereoscopic_display_start_16bit(bool during_vertical_retrace, uint32_t left_image_address_byte, uint32_t right_image_address_byte)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = during_vertical_retrace?0x083:0x03;
    machine->regs.d.ecx = left_image_address_byte;
    machine->regs.d.edx = right_image_address_byte;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_get_scheduled_display_start_16bit(bool* flip_occured)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = 0x04;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *flip_occured = machine->regs.x.cx==0?false:true;
    return VBE_OK;
}

VBEStatus VBE_enable_stereoscopic_mode()
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = 0x05;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_disable_stereoscopic_mode()
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f07;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = 0x06;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_set_display_start_32bit(bool during_vertical_retrace, uint16_t first_half_start_address, uint16_t second_half_start_address, uint16_t memory_selector)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.h.bh = 0x00;
    machine->regs.h.bl = during_vertical_retrace?0x80:0x00;
    machine->regs.x.cx = first_half_start_address;
    machine->regs.x.dx = second_half_start_address;
    machine->sregs.es = memory_selector;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_set_dac_palette_format(uint8_t primary_color_bits, uint8_t* curent_number_color_bits)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f08;
    machine->regs.h.bl = 0x00;
    machine->regs.h.bh = primary_color_bits;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *curent_number_color_bits = machine->regs.h.bh;
    return VBE_OK;
}

VBEStatus VBE_get_dac_palette_format(uint8_t* curent_number_color_bits)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f08;
    machine->regs.h.bl = 0x01;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *curent_number_color_bits = machine->regs.h.bh;
    return VBE_OK;
}

VBEStatus VBE_set_palette_data_16bit(bool secondary_palette, uint8_t index, uint8_t palette_table[], uint16_t table_size)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f09;
    machine->regs.h.bl = secondary_palette?0x02:0x00;
    machine->regs.x.cx = table_size;
    machine->regs.x.dx = index;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x7E00;
    for(uint16_t i = 0; i<table_size; i++)
    {
        machine->Memory[0x00*10+0x7E00 + i] = palette_table[i];
    }
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_set_during_vertical_retrace_palette_data_16bit(uint16_t index, uint8_t palette_table[], uint16_t table_size)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f09;
    machine->regs.h.bl = 0x80;
    machine->regs.x.cx = table_size;
    machine->regs.x.dx = index;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x7E00;
    for(uint16_t i = 0; i<table_size; i++)
    {
        machine->Memory[0x00*10+0x7E00 + i] = palette_table[i];
    }
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_get_palette_data_16bit(bool secondary_palette, uint16_t index, uint8_t palette_table[], uint16_t table_size)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f09;
    machine->regs.h.bl = secondary_palette?0x03:0x01;
    machine->regs.x.cx = table_size;
    machine->regs.x.dx = index;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x7E00;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    for(uint16_t i = 0; i<table_size; i++)
    {
        palette_table[i] = machine->Memory[0x00 * 10 + 0x7E00 + i];
    }
    return VBE_OK;
}

VBEStatus VBE_set_palette_data_32bit(uint16_t index, uint8_t palette_table[], uint16_t table_size, uint16_t memory_selector)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.h.bl = 0x00;
    machine->regs.x.cx = table_size;
    machine->regs.x.dx = index;
    machine->sregs.es = 0x0000;
    machine->regs.d.edi = 0x7E00;
    machine->sregs.ds = memory_selector;
    for(uint16_t i = 0; i<table_size; i++)
    {
        machine->Memory[0x00*10+0x7E00 + i] = palette_table[i];
    }
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}


VBEStatus VBE_set_during_vertical_retrace_palette_data_32bit(uint16_t index, uint8_t palette_table[], uint16_t table_size, uint16_t memory_selector)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.h.bl = 0x80;
    machine->regs.x.cx = table_size;
    machine->regs.x.dx = index;
    machine->sregs.es = 0x0000;
    machine->regs.d.edi = 0x7E00;
    machine->sregs.ds = memory_selector;
    for(uint16_t i = 0; i<table_size; i++)
    {
        machine->Memory[0x00*10+0x7E00 + i] = palette_table[i];
    }
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    return VBE_OK;
}

VBEStatus VBE_get_protected_mode_interface(uint16_t* real_mode_table_segment, uint16_t* table_offset, uint16_t* table_length)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f0a;
    machine->regs.h.bl = 0x01;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *real_mode_table_segment = machine->sregs.es;
    *table_offset = machine->regs.x.di;
    *table_length = machine->regs.x.cx;
    return VBE_OK;
}

VBEStatus VBE_get_set_pixel_clock(uint16_t pixel_clock, uint16_t mode_number, uint16_t* closest_pixel_clock)
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f0b;
    machine->regs.h.bl = 0x00;
    machine->regs.d.ecx = pixel_clock;
    machine->regs.x.dx = mode_number;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *closest_pixel_clock = machine->regs.d.ecx;
    return VBE_OK;
}

void VBE_draw_pixel_8_8_8(uint32_t mode_width, uint32_t mode_height, uint32_t winsize, uint32_t granularity, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t offset = (y * 3) * mode_width + (x * 3);
    uint32_t position = offset / (granularity * 1024);
    offset = offset - (granularity * 1024) * position;
    if(position != currentBank)
    {
        VBEStatus status = VBE_display_window_control_set_16bit(0, position);
        currentBank = position;
    }
    mem_buff[offset] = b;
    mem_buff[offset + 1] = g;
    mem_buff[offset + 2] = r;
}

void VBE_draw_pixel_8_8_8_linear(uint32_t mode_width, uint32_t mode_height, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (linear_buffer != 0)
    {
        uint32_t index = 3 * mode_width * y + 3 * x;
        linear_buffer[index] = b;
        linear_buffer[index+1] = g;
        linear_buffer[index+2] = r;
    }
}

void VBE_draw_pixel_8_8_8_8_linear(uint32_t mode_width, uint32_t mode_height, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t s)
{
    if (linear_buffer != 0)
    {
        uint32_t index = 4 * mode_width * y + 4 * x;
        linear_buffer[index] = b;
        linear_buffer[index+1] = g;
        linear_buffer[index+2] = r;
        linear_buffer[index+3] = s;
    }
}


//videocard driver implementation
// Set video mode
// IMPORTANT TODO: Add safety return to mode 03h if mode switch failed.
// TODO: Add warning about SVGA driver being in work, so you can bypass monitor
// resolution support check, without ANY LIABILITY to authors of this OS and/or driver.
int16_t __vbe_set_video_mode(uint16_t mode)
{
    video_mode* mode_ptr = video_card_find_mode_by_number(mode);
    if(mode_ptr == NULL)
    {
        return -1;
    }
    else
    {
        VBE_set_video_mode(mode|(mode_ptr->planar<<14), true);
    }
}
        // for(uint32_t i = 0; i < monitor_res->count; i++)
        // {
        //     VBE_resolution* res = (VBE_resolution*)monitor_res->data[i];
        //     if(res->horizontal == mode_ptr->width && res->vertical == mode_ptr->height)
        //     {
        //         VBE_set_video_mode(mode|(mode_ptr->planar<<14), true);
        //         return 0;
        //     }
        // }
    //     char buff[128];
    //     VBE_set_video_mode(0x03, true);
    //     vga_printstring("IMPORTANT!\n");
    //     vga_printstring("SVGA driver is still in development and cannot ensure full support of\nrequested resolution:\n");
    //     video_mode* mode_ptr = video_card_find_mode_by_number(mode);
    //     kernel_sprintf(buff, "%d x %d %d BPP\n", mode_ptr->width, mode_ptr->height, mode_ptr->bpp);
    //     vga_printstring(buff);
    //     vga_printstring("If you see \"Out of range\" press ESC key or wait for 15 seconds.\n");
    //     vga_printstring("If it displays anything (even weird mixed colors) it should be fine.\n");
    //     vga_printstring("To confirm successful switch press ENTER. Otherwise it return to text mode.\n");
    //     vga_printstring("THIS WARNING IS DUE TO DISPLAY NOT LISTING REQUESTED RESOLTION AS SUPPORTED!\n");
    //     vga_printstring("SWITCHING TO UNSUPPORTED RESOLUTION MAY DAMAGE YOUR DISPLAY (CRT).\n");
    //     vga_printstring("Proceed at your own risk.\n");
    //     vga_printstring("If you wish to continue press Y, to cancel switch press N.\n");

    //     keyboard_scan_ascii_pair kb;

    //     while(1)
    //     {
    //         if(keyboard_get_key_from_buffer(&kb))
    //         {
    //             if(kb.ascii == 'n') return -1;
    //             else if(kb.ascii == 'y') break;
    //         }
    //     }

    //     VBE_set_video_mode(mode|(mode_ptr->planar<<14), true);

    //     uint32_t current_system_clock = timer_get_system_clock();
    //     while(timer_get_system_clock < current_system_clock+15000)
    //     {
    //         if(keyboard_get_key_from_buffer(&kb))
    //         {
    //             if(kb.scancode = 0x01)
    //             {
    //                 VBE_set_video_mode(0x03, false);
    //                 return -1;
    //             }
    //             else if(kb.scancode == 0x1c) return 0;
    //         }
    //     }
    //     VBE_set_video_mode(0x03, false);
    //     return -1;
    // }
    // return -1;

// These want to use memory mapped for VGA modes. We cannot use it since any SVGA mode
// will need bigger memory area.
int8_t __vbe_turn_on_buffer()
{
    return -1;
}

int8_t __vbe_turn_off_buffer()
{
    // if(page_index != 0)
    // {
    //     for(int i = 0; i < page_count+1; i++)
    //     {
    //         paging_unmap_page(page_index+i);
    //     }
    //     page_index = 0;
    //     page_count = 0;
    //     linear_buffer = 0;
    // }
    //This is workaround for a weird case of VBE mode not allowing returning back to standard VGA mode.
    //This way we just make sure that VBE switched back to standard VGA mode and it unlocks normal VGA register.
    //TODO: After DAL rework this should be changed to make proper use of VBE functions etc.
    VBE_set_video_mode(0x03, true);
    return 0;
}
uint8_t __vbe_is_buffer_on()
{
    return -1;
}

int8_t __vbe_swap_buffers()
{
    return -1;
}

//Text mode functions (NOT IMPLEMENTED)
int8_t __vbe_print_char(char character)
{
    return -1;
}

int8_t __vbe_print_char_color(char character, uint8_t color)
{
    return -1;
}

int8_t __vbe_print_string(const char* string)
{
    return -1;
}

int8_t __vbe_print_string_color(const char* string, uint8_t color)
{
    return -1;
}

int8_t __vbe_set_char(uint16_t x, uint16_t y, char character)
{
    return -1;
}

int8_t __vbe_get_char(uint16_t x, uint16_t y, char* character)
{
    return -1;
}

int8_t __vbe_set_color(uint16_t x, uint16_t y, uint8_t color)
{
    return -1;
}

int8_t __vbe_get_color(uint16_t x, uint16_t y, uint8_t* color)
{
    return -1;
}

int8_t __vbe_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color)
{
    return -1;
}

int8_t __vbe_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color)
{
    return -1;
}

int8_t __vbe_set_cursor_pos(uint16_t x, uint16_t y)
{
    return -1;
}

int8_t __vbe_get_cursor_pos(uint16_t* x, uint16_t* y)
{
    return -1;
}

int8_t __vbe_turn_cursor_on()
{
    return -1;
}

int8_t __vbe_turn_cursor_off()
{
    return -1;
}

int8_t __vbe_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character)
{
    return -1;
}

int8_t __vbe_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color)
{
    return -1;
}

int8_t __vbe_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* character)
{
    return -1;
}

int8_t __vbe_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* character, uint8_t  color)
{
    return -1;
}

int8_t __vbe_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character)
{
    return -1;
}

int8_t __vbe_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character)
{
    return -1;
}

int8_t __vbe_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color)
{
    return -1;
}

int8_t __vbe_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color)
{
    return -1;
}

int8_t __vbe_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color)
{
    return -1;
}

int8_t __vbe_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color)
{
    return -1;
}

// Graphic functions, all not implemented, dal architecture makes it impossible
// To implement them in other driver than standard VGA.

//Not supported, mostly due to wrong DAL architecture, will get fixed after rework of DAL in 2022.
int8_t __vbe_draw_pixel(uint8_t color, uint16_t x, uint16_t y)
{
    return -1;
}

int8_t __vbe_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}

int8_t __vbe_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return -1;
}

int8_t __vbe_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}

int8_t __vbe_clear_screen()
{
    return -1;
}

//Not supported, mostly due to wrong DAL architecture, will get fixed after rework of DAL in 2022.
int8_t __vbe_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y)
{
    return -1;
}

int8_t __vbe_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}

int8_t __vbe_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return -1;
}

int8_t __vbe_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}

int8_t __vbe_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y)
{
    memset(buffer, 0, linear_buff_size);
    return 0;
}

//Implemented

int8_t __vbe_swap_external_buffer(uint8_t* buffer, uint16_t mode)
{
    if(linear_buffer != 0)
    {
        video_mode* mode_ptr = video_card_find_mode_by_number(mode);
        memcpy(linear_buffer, buffer, mode_ptr->linear_buffer_size);
    }
    return 0;
}

uint8_t* __vbe_create_external_buffer(uint16_t mode)
{
    video_mode* mode_ptr = video_card_find_mode_by_number(mode);
    uint8_t* ptr = heap_kernel_alloc(mode_ptr->linear_buffer_size, 0);
    return ptr;
}

void __vbe_destroy_external_buffer(uint8_t* buffer)
{
    heap_kernel_dealloc(buffer);
}


