#ifndef VBE_H
#define VBE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

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
} svga_mode_information;

typedef enum _VBEStatus{
    VBE_OK, VBE_NOT_EXIST, VBE_FUNCTION_FAILURE, VBE_NO_INITAILIZED, VBE_INTERNAL_ERROR
} VBEStatus;


void VBE_initialize();
void VBE_close();
bool VBE_is_initialized();
VBEStatus VBE_check_existance_of_VESA();
VBEStatus VBE_get_svga_information(svga_information** information_struct_ptr);
VBEStatus VBE_get_vesa_mode_information(svga_mode_information* infromation_struct, uint16_t mode_number);
VBEStatus VBE_destroy_svga_information(svga_information* svga_information_ptr);
VBEStatus VBE_set_video_mode(uint16_t mode_number, bool clear_screen);
VBEStatus VBE_get_current_video_mode(uint16_t* mode_number);
VBEStatus VBE_return_save_restore_state_buffer_size(uint16_t requested_states, uint16_t* buffer_block_number);
VBEStatus VBE_save_state(uint16_t requested_states, uint16_t buffer_pointer);
VBEStatus VBE_restore_state(uint16_t requested_states, uint16_t buffer_pointer);
VBEStatus VBE_display_window_control_set_16bit(uint8_t window_number, uint8_t window_mem_number);
VBEStatus VBE_display_window_control_get_16bit(uint8_t window_number, uint8_t* window_mem_number);
VBEStatus VBE_display_window_control_set_32bit(uint8_t window_number, uint8_t window_mem_number, uint16_t memory_selector);
VBEStatus VBE_get_logical_scan_line_length(bool get_maximum_length, uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number);
VBEStatus VBE_set_logical_scan_line_length(bool in_pixels, uint16_t length,uint16_t* bytes_per_line, uint16_t* actual_pixel_in_line, uint16_t* maximum_scan_lines_number);
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

#endif