#ifndef V8086_MOD_RM_PARSING_H
#define V8086_MOD_RM_PARSING_H

#include "v8086.h"

#define get_byte_register(machine, reg_field) ((uint8_t*)(&(machine->regs.h.al) + ((((reg_field) & 3) << 2) + ((reg_field) >> 2))))
#define get_word_register(machine, reg_field) ((uint16_t*)(&(machine->regs.d.eax) + (reg_field)))
#define get_dword_register(machine, reg_field) ((uint32_t*)(&(machine->regs.d.eax) + (reg_field)))

/*uint8_t* get_byte_register(v8086* machine, uint8_t reg_field);
uint16_t* get_word_register(v8086* machine, uint8_t reg_field);
uint32_t* get_dword_register(v8086* machine, uint8_t reg_field);*/
void* get_variable_length_register(v8086* machine, uint8_t reg_field, uint8_t width);

#define select_segment_register(machine, select) ((uint16_t*)(&((machine)->sregs.es) + (select)))

//uint16_t* select_segment_register(v8086* machine, segment_register_select select);
int16_t calculate_segment_offset_from_mode(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint32_t* offset);
int16_t calculate_segment_offset_from_mode_32(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint32_t* offset);
void* get_memory_from_mode(v8086* machine, uint8_t mod_rm, uint8_t width);

#endif //V8086_MOD_RM_PARSING_H
