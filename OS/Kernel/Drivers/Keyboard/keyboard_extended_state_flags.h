#ifndef KEYBOARD_EXTENDED_STATE_FLAGS_H
#define KEYBOARD_EXTENDED_STATE_FLAGS_H

typedef struct keyboard_extended_state_flags
{
    char last_E1h : 1;
    char last_E0h : 1;
    char right_ctrl_pressed : 1;
    char right_alt_pressed : 1;
    char ps2_keyboard_exist : 1;
    char needed_num_lock : 1;
    char last_identification_code : 1;
    char reading_code_pending : 1;
    char scroll_lock_led : 1;
    char num_lock_led : 1;
    char caps_lock_led : 1;
    char reserved : 1;
    char recived_ack : 1;
    char sent_ack : 1;
    char led_refresh_pending : 1;
    char data_error : 1;
} keyboard_extended_state_flags;

#endif