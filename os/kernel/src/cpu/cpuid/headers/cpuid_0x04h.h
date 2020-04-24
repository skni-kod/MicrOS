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

//! System Coherency Line Size, Physical Line partitions and Ways of associativity.
typedef struct cpuid_0x04h_ebx_fields
{
    //! System Coherency Line Size.
    /*! Add one to the return value to get the result.*/
    uint16_t l : 12;
    //! Physical Line partitions.
    /*! Add one to the return value to get the result.*/
    uint16_t p : 10;
    //! Ways of associativity.
    /*! Add one to the return value to get the result.*/
    uint16_t w : 10;
} __attribute__((packed)) cpuid_0x04h_ebx_fields;

//! Number of Sets.
typedef struct cpuid_0x04h_ecx_fields
{
    //! Number of Sets.
    /*! Add one to the return value to get the result.*/
    uint32_t s;
} __attribute__((packed)) cpuid_0x04h_ecx_fields;

//! Write-Back Invalidate/Invalidate, Cache Inclusiveness, Complex Cache Indexing.
typedef struct cpuid_0x04h_edx_fields
{
    //! Write-Back Invalidate/Invalidate.
    /*! 0 = WBINVD/INVD from threads sharing this cache acts upon lower level caches for threads sharing this cache.<br/>
    1 = WBINVD/INVD is not guaranteed to act upon lower level caches of non-originating threads sharing this cache.*/
    uint8_t write_back : 1;
    //! Cache Inclusiveness.
    /*!0 = Cache is not inclusive of lower cache levels.<br/>
    1 = Cache is inclusive of lower cache levels.*/
    uint8_t cache_inclusiveness : 1;
    //! Complex Cache Indexing.
    /*!0 = Direct mapped cache.<br/>
    1 = A complex function is used to index the cache, potentially using all address bits.*/
    uint8_t copmlex_cache_indexing : 1;
    // Reserved
    uint32_t : 29;
} __attribute__((packed)) cpuid_0x04h_edx_fields;

//! Intel thread/core and cache topology.
typedef struct cpuid_0x04h_fields
{
    //! EAX fields.
    cpuid_0x04h_eax_fields eax;
    //! EBX fields.
    cpuid_0x04h_ebx_fields ebx;
    //! ECX fields.
    cpuid_0x04h_ecx_fields ecx;
    //! EDX fields.
    cpuid_0x04h_edx_fields edx;
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