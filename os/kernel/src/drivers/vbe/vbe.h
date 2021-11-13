/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * THIS FILE IS MARKED TO BE REFACTORED IN FUTURE. TRY NOT TO ADD NEW THINGS TO PARTS OF OS IN THIS FILE *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VBE_H
#define VBE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../../klibrary/kvector.h"

#define DDC1_SUPPORT 0x01
#define DDC2_SUPPORT 0x02
#define BLANK_DURING_TRANSFER 0x04

typedef struct _svga_information{
    char signature[5];
    uint16_t vesa_standard_number;
    char* producent_text;
    uint32_t additional_info;
    uint16_t* mode_array;
    size_t number_of_modes;
} svga_information;

typedef struct _svga_mode_information{
    uint16_t mode_attributes;
    uint8_t window_a_attributes;
    uint8_t window_b_attributes;
    uint16_t granularity;
    uint16_t windows_size;
    uint16_t segment_of_window_a;
    uint16_t segment_of_window_b;
    uint32_t far_pointer_to_bank_swap_function;
    uint16_t logical_line_length;
    uint16_t mode_width;
    uint16_t mode_height;
    uint8_t char_matrix_width;
    uint8_t char_matrix_height;
    uint8_t plane_count;
    uint8_t bits_per_pixel;
    uint8_t bank_count;
    uint8_t memory_organization;
    uint8_t bank_size;
    uint8_t page_count;
    uint8_t reserved;
    
    uint8_t mask_size_red;
    uint8_t field_position_red;
    uint8_t mask_size_green;
    uint8_t field_position_green;
    uint8_t mask_size_blue;
    uint8_t field_position_blue;
    uint8_t mask_size_direct_color;
    uint8_t field_position_direct_color;
    uint8_t direct_color_mode_info;

    uint32_t frame_buffor_phys_address;
    uint32_t reserved1;
    uint16_t reserved2;
} svga_mode_information;

typedef struct _edid
{
    uint8_t header[8];
    uint16_t manufacturer_name;
    uint16_t product_code;
    uint32_t serial_number;
    uint8_t manufacture_week;
    uint8_t manufacture_year;
    uint8_t edid_version;
    uint8_t edid_revision;
    uint8_t video_input_type;
    uint8_t max_horizontal_image_size; //in cm!
    uint8_t max_vertical_image_size; //in cm!
    uint8_t display_gamma;
    uint8_t supported_features;
    //Color characteristics
    uint8_t color_characteristics[10];
    uint8_t established_supported_timings[2];
    uint8_t manufacturer_reserved_timing;
    uint16_t standard_timings_supported[8];
    uint8_t detailed_timing_desc[72];
    uint8_t extension_num;                    //Number of (optional) 128-byte EDID extension blocks to follow
    uint8_t checksum;
} __attribute__((packed)) edid;

typedef struct _VESA_std_timing
{
    uint16_t horizontal_res;
    uint16_t vertical_res;
    uint8_t aspect_ratio;
    uint8_t refresh_rate;
} VESA_std_timing;

typedef struct _VESA_detailed_timing
{
    uint16_t pixel_clock : 16;
    uint16_t horizontal_active;
    uint16_t horizontal_blanking;
    uint16_t vertical_active;
    uint16_t vertical_blanking;
    uint16_t hsync_offset;
    uint16_t hsync_pulse_width;
    uint16_t vsync_offset;
    uint16_t vsync_pulse_width;
    uint16_t horizontal_image_size;
    uint16_t vertical_image_size;
    uint8_t horizontal_border;
    uint8_t vertical_border;
    uint8_t flags;
} VESA_detailed_timing;

//This enum should be filled with new Descriptors that are added outside of EDID specification.
//See VESA-EEDID-A2 for more ingormation.
enum VESA_DescriptorDataType
{
    UNUSED = 0x10,
    ESTABLISHED_TIMINGS_III = 0xF7,
    STANDARD_TIMINGS = 0xFA,
    MONITOR_NAME = 0xFC,
    MONITOR_RANGE_LIMITS = 0xFD,
    ASCII_STRING = 0xFE,
    MONITOR_SN = 0xFF,
};

typedef struct _VESA_monitor_descriptor
{
    uint16_t flag;
    uint8_t reserved;
    uint8_t data_type;
    uint8_t;
    uint8_t data[13];
} __attribute__((packed)) VESA_monitor_descriptor;

typedef enum _VBEStatus
{
    VBE_OK,
    VBE_NOT_EXIST,
    VBE_FUNCTION_FAILURE,
    VBE_NO_INITAILIZED,
    VBE_INTERNAL_ERROR
} VBEStatus;

//TODO Add timing calculation to let choose other refresh rate. For now use defaults provided by BIOS.
typedef struct _VBE_resolution
{
    uint16_t vertical;
    uint16_t horizontal;
    uint16_t refresh_rate;
    uint32_t pixel_clock;
} VBE_resolution;

void VBE_initialize();
void VBE_close();
bool VBE_is_initialized();

void VBE_init_driver();

VBEStatus VBE_check_existance_of_VESA();
VBEStatus VBE_get_svga_information(svga_information** information_struct_ptr);
VBEStatus VBE_get_vesa_mode_information(svga_mode_information* infromation_struct, uint16_t mode_number);

VBEStatus VBE_DDC_capabilities();
VBEStatus VBE_DDC_read_edid_block(uint16_t block_number, edid* block_ptr);
VESA_std_timing VBE_DDC_decode_std_timing(uint16_t timing);
VESA_detailed_timing VBE_DDC_read_detailed_timing(edid* block_ptr, uint8_t detailed_block_num);
VESA_monitor_descriptor VBE_DDC_read_monitor_descriptor(edid* block_ptr, uint8_t block_num);

/**
 * @todo Add clock calculations 
 */
VBE_resolution VBE_create_resolution_def(uint16_t horizontal, uint16_t vertical, uint16_t refresh_rate);

VBEStatus VBE_destroy_svga_information(svga_information* svga_information_ptr);
VBEStatus VBE_set_video_mode(uint16_t mode_number, bool clear_screen);
VBEStatus VBE_get_current_video_mode(uint16_t* mode_number);
uint16_t VBE_get_word(uint32_t seg, uint32_t offset);
uint32_t VBE_get_dword(uint32_t seg, uint32_t offset);
VBEStatus VBE_return_save_restore_state_buffer_size(uint16_t requested_states, uint16_t* buffer_block_number);
VBEStatus VBE_save_state(uint16_t requested_states, uint16_t buffer_pointer);
VBEStatus VBE_restore_state(uint16_t requested_states, uint16_t buffer_pointer);
VBEStatus VBE_display_window_control_set_16bit(uint8_t window_number, uint8_t window_mem_number);
VBEStatus VBE_display_window_control_get_16bit(uint8_t window_number, uint8_t* window_mem_number);
VBEStatus VBE_display_window_control_set_32bit(uint8_t window_number, uint8_t window_mem_number, uint16_t memory_selector);
VBEStatus VBE_get_logical_scan_line_length(uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number);
VBEStatus VBE_get_maximum_logical_scan_line_length(uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number);
VBEStatus VBE_set_logical_scan_line_length_in_pixels(uint16_t length, uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number);
VBEStatus VBE_set_logical_scan_line_length_in_bytes(uint16_t length, uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number);
VBEStatus VBE_set_display_start_16bit(bool during_vertical_retrace, uint16_t first_line, uint16_t first_pixel_in_line);
VBEStatus VBE_get_display_start_16bit(uint16_t* first_line, uint16_t* first_pixel_in_line);
VBEStatus VBE_schedule_display_start_16bit(bool during_vertical_retrace, uint32_t display_start_address_byte);
VBEStatus VBE_schedule_stereoscopic_display_start_16bit(bool during_vertical_retrace, uint32_t left_image_address_byte, uint32_t right_image_address_byte);
VBEStatus VBE_get_scheduled_display_start_16bit(bool* flip_occured);
VBEStatus VBE_enable_stereoscopic_mode();
VBEStatus VBE_disable_stereoscopic_mode();
VBEStatus VBE_set_display_start_32bit(bool during_vertical_retrace, uint16_t first_half_start_address, uint16_t second_half_start_address, uint16_t memory_selector);
VBEStatus VBE_set_dac_palette_format(uint8_t primary_color_bits, uint8_t* curent_number_color_bits);
VBEStatus VBE_get_dac_palette_format(uint8_t* curent_number_color_bits);
VBEStatus VBE_set_palette_data_16bit(bool secondary_palette, uint8_t index, uint8_t palette_table[], uint16_t table_size);
VBEStatus VBE_set_during_vertical_retrace_palette_data_16bit(uint16_t index, uint8_t palette_table[], uint16_t table_size);
VBEStatus VBE_get_palette_data_16bit(bool secondary_palette, uint16_t index, uint8_t palette_table[], uint16_t table_size);
VBEStatus VBE_set_palette_data_32bit(uint16_t index, uint8_t palette_table[], uint16_t table_size, uint16_t memory_selector);
VBEStatus VBE_set_during_vertical_retrace_palette_data_32bit(uint16_t index, uint8_t palette_table[], uint16_t table_size, uint16_t memory_selector);
VBEStatus VBE_get_protected_mode_interface(uint16_t* real_mode_table_segment, uint16_t* table_offset, uint16_t* table_length);
VBEStatus VBE_get_set_pixel_clock(uint16_t pixel_clock, uint16_t mode_number, uint16_t* closest_pixel_clock);

//Implementation of videocard generic driver.
//Not supported features are supposed to return -1.
//For now all text based functions will return -1.

// DAL is incorrectly designed, hence anything else than
// standard VGA modes won't work at all.
// This means we're going to implement ONLY swap external buffer.
// This makes sure we can draw into VBE buffer without any problem.
// All drawing has to be offloaded to application.
// NO PRIMITIVE DRAWING ROUTINES ARE AVAILABLE.
// DAL IS SUPPOSED TO BE REWORKED IN 2022 OR AROUND CHRISTMAS 2021.


//video_mode* (*get_available_graphic_video_modes)(uint32_t*);
//video_mode* (*get_available_text_video_modes)(uint32_t*);

int16_t __vbe_set_video_mode(uint16_t mode);

//Not implemented, VBE need buffer larger than available memory.
int8_t __vbe_turn_on_buffer();
int8_t __vbe_turn_off_buffer();
uint8_t __vbe_is_buffer_on();
int8_t __vbe_swap_buffers();

// Text mode functions
// Since VBE is used for entering graphic modes these for now can just return -1
// In future if we're even going to support text modes greater than 80x25
// which is standard 03h mode from vga, we'll need to implement them.
// but more likely case is that we're going to implement vendor/device
// specific driver.
int8_t __vbe_print_char(char character);
int8_t __vbe_print_char_color(char character, uint8_t color);
int8_t __vbe_print_string(const char* string);
int8_t __vbe_print_string_color(const char* string, uint8_t color);
int8_t __vbe_set_char(uint16_t x, uint16_t y, char character);
int8_t __vbe_get_char(uint16_t x, uint16_t y, char* character);
int8_t __vbe_set_color(uint16_t x, uint16_t y, uint8_t color);
int8_t __vbe_get_color(uint16_t x, uint16_t y, uint8_t* color);
int8_t __vbe_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color);
int8_t __vbe_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color);
int8_t __vbe_set_cursor_pos(uint16_t x, uint16_t y);
int8_t __vbe_get_cursor_pos(uint16_t* x, uint16_t* y);
int8_t __vbe_turn_cursor_on();
int8_t __vbe_turn_cursor_off();
int8_t __vbe_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character);
int8_t __vbe_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color);
int8_t __vbe_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* character);
int8_t __vbe_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* character, uint8_t  color);
int8_t __vbe_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character);
int8_t __vbe_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character);
int8_t __vbe_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color);
int8_t __vbe_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color);
int8_t __vbe_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color);
int8_t __vbe_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t*);

//Not supported, mostly due to wrong DAL architecture, will get fixed after rework of DAL in 2022.
int8_t __vbe_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t __vbe_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t __vbe_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t __vbe_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t __vbe_clear_screen();
//Not supported, mostly due to wrong DAL architecture, will get fixed after rework of DAL in 2022.
int8_t __vbe_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t __vbe_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t __vbe_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t __vbe_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t __vbe_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t __vbe_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* __vbe_create_external_buffer(uint16_t mode);
void __vbe_destroy_external_buffer(uint8_t* buffer);

//Leftovers for debug, should be removed. This way of drawing is slow.
void VBE_draw_pixel_8_8_8(uint32_t mode_width, uint32_t mode_height, uint32_t winsize, uint32_t granularity, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void VBE_draw_pixel_8_8_8_linear(uint32_t mode_width, uint32_t mode_height, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void VBE_draw_pixel_8_8_8_8_linear(uint32_t mode_width, uint32_t mode_height, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t s);




#endif