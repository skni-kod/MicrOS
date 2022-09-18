#ifndef CPUID_0x00H_H
#define CPUID_0x00H_H

#include <stdint.h>

//! CPU highest function parameter and manufacturer id.
typedef struct cpuid_0x00h_fields
{
    //! CPU highest function parameter.
    uint32_t highest_function_parameter;
    //! CPU manufacturer id 1.
    char manufacturer_id_1[4];
    //! CPU manufacturer id 2.
    char manufacturer_id_2[4];
    //! CPU manufacturer id 3.
    char manufacturer_id_3[4];
} __attribute__((packed)) cpuid_0x00h_fields;


//! CPU highest function parameter and manufacturer id.
typedef union cpuid_0x00h
{
    //! Value of union
    uint32_t value[4];
    //! Separate fields.
    cpuid_0x00h_fields fields;
} cpuid_0x00h;

#endif