/*
    @JakubPrzystasz
    Created: 13.02.2021
*/
#ifndef network_utils
#define network_utils

#include <stdint.h>

//! __uint8_flip
/*
    Flip bits in 8 bit-wide field
*/
uint8_t __uint8_flip(uint8_t value);
//! __uint16_flip
/*
    Flip bits in 16 bit-wide field
*/
uint16_t __uint16_flip(uint16_t value);

//! __uint32_flip
/*
    Flip bits in 32 bit-wide field
*/
uint32_t __uint32_flip(uint32_t value);

#endif