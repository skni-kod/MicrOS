#ifndef PANIC_SCREEN_H
#define PANIC_SCREEN_H

#include <stdlib.h>
#include <stdint.h>
#include "../Drivers/VGA/vga.h"
#include "../Drivers/Keyboard/keyboard.h"
#include "../Library/kvector.h"

void panic_screen_show(exception_state *state, uint32_t code, const char *optString);
void panic_screen_display_intro(exception_state *state, uint32_t code, const char *optString);
void panic_screen_wait_for_key_press();
void panic_screen_display_diagnostic_view(exception_state *state);
char *panic_screen_value_to_string(char *buffer, unsigned int value);

void panic_screen_display_eflags(uint32_t eflags);
void panic_screen_display_cr0(uint32_t cr0);
void panic_screen_display_cr4(uint32_t cr4);
#endif