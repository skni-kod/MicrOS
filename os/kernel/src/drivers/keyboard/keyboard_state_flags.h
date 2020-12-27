#ifndef KEYBOARD_STATE_FLAGS_H
#define KEYBOARD_STATE_FLAGS_H

typedef struct keyboard_state_flags
{
    uint8_t right_shift_pressed : 1;
    uint8_t left_shift_pressed : 1;
    uint8_t ctrl_pressed : 1;
    uint8_t alt_pressed : 1;
    uint8_t scroll_lock_active : 1;
    uint8_t num_lock_active : 1;
    uint8_t caps_lock_active : 1;
    uint8_t insert_active : 1;
    uint8_t left_ctrl_pressed : 1;
    uint8_t left_alt_pressed : 1;
    uint8_t sys_req_pressed : 1;
    uint8_t pause_active : 1;
    uint8_t scroll_lock_pressed : 1;
    uint8_t num_lock_pressed : 1;
    uint8_t caps_lock_pressed : 1;
    uint8_t insert_pressed : 1;
} keyboard_state_flags;

#endif