#ifndef KEYBOARD_H /* Include guard */
#define KEYBOARD_H

#include "cpu/idt/idt.h"
#include "cpu/pic/pic.h"
#include "keyboard_extended_state_flags.h"
#include "keyboard_state_flags.h"
#include "keyboard_scan_acii_pair.h"

void keyboard_init();
unsigned char keyboard_able_to_write();
unsigned char keyboard_is_buffer_empty();
bool keyboard_get_key_state(char scancode);
void keyboard_increment_buffer_pointer(volatile uint16_t *const ptr);
void keyboard_put_key_to_buffer(unsigned char scancode, unsigned char ascii);
bool keyboard_get_key_from_buffer(keyboard_scan_ascii_pair *scan_ascii_pair);
unsigned char keyboard_get_scancode();
bool keyboard_handler();
unsigned char keyboard_is_between(unsigned char num, unsigned char l, unsigned char r);
unsigned char keyboard_is_between_or_eq(unsigned char number, unsigned char l, unsigned char r);
keyboard_state_flags *keyboard_get_state_flags();

#endif