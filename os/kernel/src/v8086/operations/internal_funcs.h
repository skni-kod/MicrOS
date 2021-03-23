#ifndef V8086_INTERNAL_FUNCS_H
#define V8086_INTERNAL_FUNCS_H

#include <stdint.h>

static inline uint8_t get_reg(uint8_t mod_rm)
{
    return (uint8_t)(mod_rm >> 3u) & 7u;
}

#endif //V8086_INTERNAL_FUNCS_H
