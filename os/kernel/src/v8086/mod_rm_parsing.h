#ifndef V8086_MOD_RM_PARSING_H
#define V8086_MOD_RM_PARSING_H

#include "v8086.h"

uint8_t* get_byte_register(v8086* machine, uint8_t reg_field);
uint16_t* get_word_register(v8086* machine, uint8_t reg_field);
uint32_t* get_dword_register(v8086* machine, uint8_t reg_field);
void* get_variable_length_register(v8086* machine, uint8_t reg_field, uint8_t width);

uint16_t* select_segment_register(v8086* machine, segment_register_select select);
int16_t calculate_segment_offset_from_mode(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint32_t* offset);
int16_t calculate_segment_offset_from_mode_32(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint32_t* offset);
void* get_memory_from_mode(v8086* machine, uint8_t mod_rm, uint8_t width);

#endif //V8086_MOD_RM_PARSING_H
