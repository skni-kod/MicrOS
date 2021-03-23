#ifndef V8086_MEMORY_OPERATIONS_H
#define V8086_MEMORY_OPERATIONS_H

#include <stddef.h>
#include <stdint.h>

static inline uint32_t get_absolute_address(uint32_t segment, uint32_t offset)
{
    return segment * 0x10 + offset;
}

static inline uint8_t* get_byte_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return memory + absolute_address;
}

static inline uint16_t* get_word_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return (uint16_t*)(memory + absolute_address);
}

static inline uint32_t* get_dword_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return (uint32_t*)(memory + absolute_address);
}

static inline void* get_variable_length_pointer(uint8_t* memory, uint32_t absolute_address, uint8_t width)
{
    switch(width)
    {
        case 8:
            return get_byte_pointer(memory, absolute_address);
        case 16:
            return get_word_pointer(memory, absolute_address);
        case 32:
            return get_dword_pointer(memory, absolute_address);
        default:
            return NULL;
    }
}

static inline uint8_t read_byte_from_pointer(const uint8_t* memory, uint32_t absolute_address)
{
    return *(memory + absolute_address);
}

static inline uint16_t read_word_from_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return *get_word_pointer(memory, absolute_address);
}

static inline uint32_t read_dword_from_pointer(uint8_t* memory, uint32_t absolute_address)
{
    return *get_dword_pointer(memory, absolute_address);
}

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
