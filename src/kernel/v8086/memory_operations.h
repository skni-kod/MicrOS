#ifndef V8086_MEMORY_OPERATIONS_H
#define V8086_MEMORY_OPERATIONS_H

#include <stddef.h>
#include <stdint.h>

#define get_absolute_address(segment, offset) ((uint32_t)segment * 0x10 + (uint32_t)offset)

#define get_byte_pointer(memory, absolute_address) (memory + (uint32_t) absolute_address)

#define get_word_pointer(memory, absolute_address) ((uint16_t*)(memory + (uint32_t) absolute_address))

#define get_dword_pointer(memory, absolute_address) ((uint32_t*)(memory + (uint32_t) absolute_address))

#define get_variable_length_pointer(memory, absolute_address, width) ((void*)(memory + (uint32_t) absolute_address))

#define read_byte_from_pointer(memory, absolute_address) (*(get_byte_pointer(memory, absolute_address)))

#define read_word_from_pointer(memory, absolute_address) (*(get_word_pointer(memory, absolute_address)))

#define read_dword_from_pointer(memory, absolute_address) (*(get_dword_pointer(memory, absolute_address)))

/*
static inline uint8_t read_byte_from_pointer(const uint8_t* memory, uint32_t absolute_address)
{
    return *(memory + absolute_address);
}
*/
/*
static inline uint16_t read_word_from_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return *get_word_pointer(memory, absolute_address);
}
*/
/*
static inline uint32_t read_dword_from_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return *get_dword_pointer(memory, absolute_address);
}
*/

static inline void write_byte_to_pointer(uint8_t* memory, uint32_t absolute_address, uint8_t value)
{
    *(get_byte_pointer(memory, absolute_address)) = value;
}

static inline void write_word_to_pointer(uint8_t* memory, uint32_t absolute_address, uint16_t value)
{
    *(get_word_pointer(memory, absolute_address)) = value;
}

static inline void write_dword_to_pointer(uint8_t* memory, uint32_t absolute_address, uint32_t value)
{
    *(get_dword_pointer(memory, absolute_address)) = value;
}

#endif //V8086_MEMORY_OPERATIONS_H
