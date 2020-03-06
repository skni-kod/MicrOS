#ifndef CPUID_H
#define CPUID_H

#include "headers/cpuid_defines.h"
#include "headers/cpuid_0x00h.h"
#include "headers/cpuid_0x01h.h"
#include "headers/cpuid_0x04h.h"
#include <stdint.h>

//! Perform CPUID instruction.
/*!
    Returns processor identification and feature information to the EAX, EBX, ECX, and EDX registers, as determined by input entered in EAX (in some cases, ECX as well).
    \param code initial EAX value. Code for EAX variant.
    \param eax result from EAX.
    \param ebx result from EAX.
    \param ecx result from EAX.
    \param edx result from EAX.
*/
static inline void __cpuid_features(int code, uint32_t *eax, uint32_t *ebx, uint32_t *ecx ,uint32_t *edx)
{
    asm volatile("cpuid":"=a"(*eax),"=b"(*ebx),"=c"(*ecx),"=d"(*edx):"a"(code));
}

static inline void __cpuid(unsigned int code, uint32_t where[4]) {
     asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"a"(code));
}

//! Initialization of CPUID.
/*!
    Sets internal variables.
    \return 1 is success, 0 is failure.
*/
uint8_t cpuid_init();

//! Get vendor string.
/*!
    Function copy vendor string to buffer and terminate string with /0.
    \param buffer Buffer for vendor string with size at least 13.
    \return Pointer to buffer where serial number is stored.
*/
char* cpuid_get_vendor_string(char* buffer);

//! Get stepping id.
/*!
    \return CPU stepping id.
*/
uint8_t cpuid_get_stepping_id();

//! Get model id.
/*!
    \return CPU model id.
*/
uint8_t cpuid_get_model_id();

//! Get family id.
/*!
    \return CPU family id.
*/
uint8_t cpuid_get_family_id();

//! Get processor type.
/*!
    \return CPU processor type.
*/
uint8_t cpuid_get_processor_type();

//! Check if Hyper-Threading is available.
/*!
    \return 1 if yes, 0 if no.
*/
uint8_t cpuid_is_hyperthreading_available();

//! Get number of logical processors.
/*! Addressable IDs for Logical processors in the same Package. Valid if Hyper-Threading is enabled.
    When Hyper-Threading is not available this function returns number of processors cores.
    \return Number of addressable logical processors.
*/
uint8_t cpuid_number_of_logical_processors();

//! Helper function to get manufacturer id.
/*!
    Put vendor string in __cpuid_vendor_string variable.
*/
void __cpuid_get_manufacturer_string();


void printBrand();

#endif