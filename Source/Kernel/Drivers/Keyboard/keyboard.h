#ifndef KEYBOARD_H   /* Include guard */
#define KEYBOARD_H

#include "../../Interrupts/IDT/idt.h"

typedef struct KeyboardStateFlags{
    char right_shift_pressed:1;
    char left_shift_pressed:1;
    char ctrl_pressed:1;
    char alt_pressed:1;
    char scroll_lock_active:1;
    char num_lock_active:1;
    char caps_lock_active:1;
    char insert_active:1;
    char left_ctrl_pressed:1;
    char left_alt_pressed:1;
    char sys_req_pressed:1;
    char pause_active:1;
    char scroll_lock_pressed:1;
    char num_lock_pressed:1;
    char caps_lock_pressed:1;
    char insert_pressed:1;
} __attribute__((packed)) KeyboardStateFlags;

typedef struct KeyboardExtendedStateFlags{
    char last_E1h:1;
    char last_E0h:1;
    char right_ctrl_pressed:1;
    char right_alt_pressed:1;
    char ps2_keyboard_exist:1;
    char needed_num_lock:1;
    char last_identification_code:1;
    char reading_code_pending:1;
    char scroll_lock_led:1;
    char num_lock_led:1;
    char caps_lock_led:1;
    char reserved:1;
    char recived_ack:1;
    char sent_ack:1;
    char led_refresh_pending:1;
    char data_error:1;
} __attribute__((packed)) KeyboardExtendedStateFlags;

unsigned char get_scancode();
void keyboard_handler();
unsigned char _between(unsigned char num, unsigned char l, unsigned char r);
unsigned char _between_or_eq(unsigned char number, unsigned char l, unsigned char r);




#endif