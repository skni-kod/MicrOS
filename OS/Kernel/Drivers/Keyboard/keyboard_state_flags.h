#ifndef KEYBOARD_STATE_FLAGS_H
#define KEYBOARD_STATE_FLAGS_H

typedef struct keyboard_state_flags
{
    char right_shift_pressed : 1;
    char left_shift_pressed : 1;
    char ctrl_pressed : 1;
    char alt_pressed : 1;
    char scroll_lock_active : 1;
    char num_lock_active : 1;
    char caps_lock_active : 1;
    char insert_active : 1;
    char left_ctrl_pressed : 1;
    char left_alt_pressed : 1;
    char sys_req_pressed : 1;
    char pause_active : 1;
    char scroll_lock_pressed : 1;
    char num_lock_pressed : 1;
    char caps_lock_pressed : 1;
    char insert_pressed : 1;
} keyboard_state_flags;

#endif