#ifndef KEYBOARD_CALLS_H
#define KEYBOARD_CALLS_H

#include "../../../Drivers/Keyboard/keyboard.h"
#include "../../../Process/Manager/process_manager.h"

void syscall_keyboard_is_key_pressed(interrupt_state *state);
void syscall_keyboard_get_pressed_key(interrupt_state *state);
void syscall_keyboard_wait_for_key_press(interrupt_state *state);

#endif