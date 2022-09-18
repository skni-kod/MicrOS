#ifndef MICROS_INTERRUPTS_H
#define MICROS_INTERRUPTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Generates interrupt without passing any parameter
/*!
    Generates interrupt with the specified parameter and without passing any parameter. Then returns value stored in the eax register.
    \param function_number Function number.
    \return Value returned from the function called in the interrupt.
*/
uint32_t micros_interrupt_0a(uint32_t function_number);

//! Generates interrupt with one parameter
/*!
    Generates interrupt with the specified parameter and parameter stored in ebx register. Then returns value stored in the eax register.
    \param function_number Function number.
    \param a First parameter.
    \return Value returned from the function called in the interrupt.
*/
uint32_t micros_interrupt_1a(uint32_t function_number, uint32_t a);

//! Generates interrupt with one parameter
/*!
    Generates interrupt with the specified parameter and parameters stored in ebx and ecx registers. Then returns value stored in the eax register.
    \param function_number Function number.
    \param a First parameter.
    \param b Second parameter.
    \return Value returned from the function called in the interrupt.
*/
uint32_t micros_interrupt_2a(uint32_t function_number, uint32_t a, uint32_t b);

//! Generates interrupt with one parameter
/*!
    Generates interrupt with the specified parameter and parameters stored in ebx, ecx and edx registers. Then returns value stored in the eax register.
    \param function_number Function number.
    \param a First parameter.
    \param b Second parameter.
    \param c Third parameter.
    \return Value returned from the function called in the interrupt.
*/
uint32_t micros_interrupt_3a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c);

//! Generates interrupt with one parameter
/*!
    Generates interrupt with the specified parameter and parameters stored in ebx, ecx, edx and esi registers. Then returns value stored in the eax register.
    \param function_number Function number.
    \param a First parameter.
    \param b Second parameter.
    \param c Third parameter.
    \param d Fourth parameter.
    \return Value returned from the function called in the interrupt.
*/
uint32_t micros_interrupt_4a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

//! Generates interrupt with one parameter
/*!
    Generates interrupt with the specified parameter and parameters stored in ebx, ecx, edx, esi and edi registers. Then returns value stored in the eax register.
    \param function_number Function number.
    \param a First parameter.
    \param b Second parameter.
    \param c Third parameter.
    \param d Fourth parameter.
    \param e Fifth parameter.
    \return Value returned from the function called in the interrupt.
*/
uint32_t micros_interrupt_5a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);

#ifdef __cplusplus
}
#endif

#endif