#ifndef KEYBOARD_CALLS_H
#define KEYBOARD_CALLS_H

#include "../../../Drivers/Keyboard/keyboard.h"

void syscall_keyboard_is_key_pressed(interrupt_state *state);
void syscall_keyboard_get_pressed_key(interrupt_state *state);

#endif