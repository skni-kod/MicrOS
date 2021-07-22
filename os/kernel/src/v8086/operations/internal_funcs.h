#ifndef V8086_INTERNAL_FUNCS_H
#define V8086_INTERNAL_FUNCS_H

#include <stdint.h>

#define get_reg(mod_rm) ((uint8_t)(mod_rm >> 3u) & 7u)

/*static inline uint8_t get_reg(uint8_t mod_rm)
{
    return (uint8_t)(mod_rm >> 3u) & 7u;
}*/

#endif //V8086_INTERNAL_FUNCS_H
