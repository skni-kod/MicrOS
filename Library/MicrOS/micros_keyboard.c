#include "micros_keyboard.h"

bool micros_keyboard_is_key_pressed()
{
    return micros_interrupt_0a(0x20);
}