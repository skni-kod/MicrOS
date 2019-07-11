#include "micros_keyboard.h"

bool micros_keyboard_is_key_pressed()
{
    return micros_interrupt_0a(0x20);
}

bool micros_keyboard_get_pressed_key(micros_keyboard_scan_ascii_pair *scan_ascii_pair)
{
    return micros_interrupt_1a(0x21, (uint32_t)scan_ascii_pair);
}

void micros_keyboard_wait_for_key_press(micros_keyboard_scan_ascii_pair *scan_ascii_pair)
{
    // Implementation of the keyboard interrupt system causes it's sensitive for both pressing
    // and releasing keys. That's why we need to do a little hack and skip releasing keys
    // by waiting for an empty buffer.
    
    micros_interrupt_0a(0x22);
    while (!micros_keyboard_get_pressed_key(scan_ascii_pair))
    {
        micros_interrupt_0a(0x22);
    }
}

bool micros_keyboard_get_key_state(micros_keyboard_keys scancode)
{
    return micros_interrupt_1a(0x23, scancode);
}