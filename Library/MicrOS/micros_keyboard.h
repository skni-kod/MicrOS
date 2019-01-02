#ifndef MICROS_KEYBOARD_H
#define MICROS_KEYBOARD_H

#include <stdbool.h>
#include "micros_interrupts.h"

typedef struct micros_keyboard_scan_ascii_pair
{
    unsigned char scancode;
    unsigned char ascii;
} micros_keyboard_scan_ascii_pair;

bool micros_keyboard_is_key_pressed();
bool micros_keyboard_get_pressed_key(micros_keyboard_scan_ascii_pair *scan_ascii_pair);

#endif