#ifndef CPUID_0x04H_H
#define CPUID_0x04H_H

#include <stdint.h>

//! Intel thread/core and cache topology.
typedef struct cpuid_0x04h_eax_fields
{
    //! Cache Type Field.
    /*! <table>
    <tr><th>Type</th> <th>Encoding in Binary</th></tr>
    <tr><td>No more caches.</td> <td>0</td></tr>
    <tr><td>Data Cache.</td> <td>1</td></tr>
    <tr><td>Instruction Cache.</td> <td>2</td></tr>
    <tr><td>Unified Cache.</td> <td>3</td></tr>
    <tr><td>Reserved.</td> <td>4 - 31</td></tr>
    </table>*/
    uint8_t cache_type_field : 5;
    //! Cache Level (starts at 1).
    uint8_t cache_level : 3;
    //! Self Initializing cache level (does not need SW initialization).
    uint8_t self_init_cache_level : 1;
    //! Fully Associative cache.
    uint8_t fully_associative_cache : 1;
    // Reserved
    uint8_t : 4;
    //! Maximum number of addressable IDs for logical processors sharing this cache.
    /*! Add one to the return value to get the result.*/
    uint16_t max_num_addressable_ids_logical : 12;
    //! Maximum number of addressable IDs for processor cores in the physical package.
    /*! Add one to the return value to get the result.*/
    uint8_t max_num_addressable_ids_physical : 6;
} __attribute__((packed)) cpuid_0x04h_eax_fields;

//! Intel thread/core and cache topology.
typedef struct cpuid_0x04h_fields
{
    //! EAX fields.
    cpuid_0x04h_eax_fields eax_fields;
    //! Dummy.
    uint32_t dummy[3];
} cpuid_0x04h_fields;

//! Intel thread/core and cache topology.
typedef union cpuid_0x04h
{
    //! Value of union
    uint32_t value[4];
    //! Separate fields.
    cpuid_0x04h_fields fields;
} cpuid_0x04h;

#endif