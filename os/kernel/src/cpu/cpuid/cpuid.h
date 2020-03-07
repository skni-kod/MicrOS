#ifndef CPUID_H
#define CPUID_H

#include "headers/cpuid_defines.h"
#include "headers/cpuid_0x00h.h"
#include "headers/cpuid_0x01h.h"
#include "headers/cpuid_0x04h.h"
#include <stdint.h>

//! Enum to represent cache type
typedef enum cpuid_cache_type
{
    //! Data cache
    DATA_CACHE = 0x01,
    //! Instruction cache
    INSTRUCTION_CACHE = 0x02,
    //! Unified cache
    UNIFIED_CACHE = 0x03   
} cpuid_cache_type;

//! Struct to represent cache.
typedef struct cpuid_cache_struct
{
    //! Type of cache.
    cpuid_cache_type type;
    //! Level of cache.
    uint8_t level;
    //! Size of cache in bytes.
    uint32_t size;
} cpuid_cache_struct;

//! Initialization of CPUID.
/*!
    Sets internal variables.
    \return 1 is success, 0 is failure.
*/
uint8_t cpuid_init();

//! Get highest function parameter.
/*!
    Indicates how many function parameters CPUID supports.
    You should use it to determine how much data you can aquire from CPUID functions.
    \return Highest parameter function.
*/
uint32_t cpuid_get_highest_function_parameter();

//! Get vendor string.
/*!
    Function copy vendor string to buffer and terminate string with /0.
    \param buffer Buffer for vendor string with size at least 13.
    \return Pointer to buffer where serial number is stored.
*/
char* cpuid_get_vendor_string(char* buffer);

//! Get stepping id.
/*!
    Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return CPU stepping id.
*/
uint8_t cpuid_get_stepping_id();

//! Get model id.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return CPU model id.
*/
uint8_t cpuid_get_model_id();

//! Get family id.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return CPU family id.
*/
uint8_t cpuid_get_family_id();

//! Get processor type.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return CPU processor type.
*/
uint8_t cpuid_get_processor_type();

//! Check if Hyper-Threading is available.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return 1 if yes, 0 if no.
*/
uint8_t cpuid_is_hyperthreading_available();

//! Get number of logical processors.
/*! Addressable IDs for logical processors in the same Package. Valid if Hyper-Threading is enabled.<br/>
    Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return Number of addressable logical processors.
*/
uint8_t cpuid_number_of_logical_processors();

//! Get number of physical processors cores.
/*! Addressable IDs for physical processors cores in a physical package.
    Result will be valid only if cpuid_get_highest_function_parameter() returns value 4 or higher.
    \return Number of addressable physical processors cores.
*/
uint8_t cpuid_number_of_physical_processors_cores();

//! Get number of valid cache entries.
/*! Number that determines how much cache entries are stored. Used in function cpuid_get_cache_size_in_bytes(uint8_t cache_index),
cpuid_get_cache_data(uint8_t cache_index).<br/>
Result will be valid only if cpuid_get_highest_function_parameter() returns value 4 or higher.
    \return Number of valid cache entries.
*/
uint8_t cpuid_get_valid_number_cache_entries();

//! Get cache size in bytes.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 4 or higher.
    \param cache_index Index of cache to return. To get max number use cpuid_get_valid_number_cache_entries(). If invalid 0 will be returned. 
    \return Cache size in bytes.
*/
uint32_t cpuid_get_cache_size_in_bytes(uint8_t cache_index);

//! Get cache data.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 4 or higher.
    \param cache_index Index of cache to return. To get max number use cpuid_get_valid_number_cache_entries(). If invalid 0 will be returned. 
    \return Cache data.
*/
cpuid_cache_struct cpuid_get_cache_data(uint8_t cache_index);

//! Get 0x00h fields.
/*!
    \return CPUID 0x00h fields.
*/
const cpuid_0x00h* cpuid_get_0x00h_fields();

//! Get 0x01h fields.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 1 or higher.
    \return CPUID 0x01h fields.
*/
const cpuid_0x01h* cpuid_get_0x01h_fields();

//! Get 0x04h fields.
/*! Result will be valid only if cpuid_get_highest_function_parameter() returns value 4 or higher.
    \param index Index of struct to return.
    \return CPUID 0x04h fields.
*/
const cpuid_0x04h* cpuid_get_0x04h_fields(uint8_t index);

// Helpers

//! Perform CPUID instruction.
/*!
    Returns processor identification and feature information to the EAX, EBX, ECX, and EDX registers, as determined by input entered in EAX (in some cases, ECX as well).
    \param code Initial EAX value. Code for EAX variant.
    \param eax Result from EAX.
    \param ebx Result from EAX.
    \param ecx Result from EAX.
    \param edx Result from EAX.
*/
static inline void __cpuid_features(int code, uint32_t *eax, uint32_t *ebx, uint32_t *ecx ,uint32_t *edx)
{
    asm volatile("cpuid":"=a"(*eax),"=b"(*ebx),"=c"(*ecx),"=d"(*edx):"a"(code));
}

static inline void __cpuid(unsigned int code, uint32_t where[4]) {
     asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"a"(code));
}

//! Perform CPUID instruction.
/*!
    Returns processor identification and feature information to the EAX, EBX, ECX, and EDX registers, as determined by input entered in EAX (in some cases, ECX as well).
    \param code Initial EAX value. Code for EAX variant.
    \param count Number to put in ECX..
    \param eax Result from EAX.
    \param ebx Result from EAX.
    \param ecx Result from EAX.
    \param edx Result from EAX.
*/
static inline void __cpuid_features_count(int code, int count, uint32_t *eax, uint32_t *ebx, uint32_t *ecx ,uint32_t *edx)
{
    asm volatile("cpuid":"=a"(*eax),"=b"(*ebx),"=c"(*ecx),"=d"(*edx):"a"(code), "c"(count));
}

static inline void __cpuid_count(unsigned int code, int count, uint32_t where[4]) {
     asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"a"(code), "c"(count));
}

//! Helper function to get manufacturer id.
/*!
    Put vendor string in __cpuid_vendor_string variable.
*/
void __cpuid_get_manufacturer_string();

//! Get prcessor brand.
/*!
    Function copy procesor brand  to buffer and terminate string with /0.
    \param buffer Buffer for vendor string with size at least 48.
    \return Pointer to buffer where serial number is stored.
*/
char* __cpuid_get_processor_brand(char* buffer);

#endif