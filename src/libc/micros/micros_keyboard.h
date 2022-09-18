#ifndef MICROS_KEYBOARD_H
#define MICROS_KEYBOARD_H

#include <stdbool.h>
#include "micros_interrupts.h"

//! Structure with the scancode and the ASCII representation
typedef struct micros_keyboard_scan_ascii_pair
{
    //! Scancode representing the specified key
    unsigned char scancode;
    
    //! ASCII representation of the specified key
    unsigned char ascii;
} micros_keyboard_scan_ascii_pair;

//! Represents a set of keyboard keys
typedef enum micros_keyboard_keys
{
    key_esc = 1,
    key_1 = 2,
    key_2 = 3,
    key_3 = 4,
    key_4 = 5,
    key_5 = 6,
    key_6 = 7,
    key_7 = 8,
    key_8 = 9,
    key_9 = 10,
    key_0 = 11,
    key_pause = 12,
    key_equal = 13,
    key_backspace = 14,
    key_tab = 15,
    key_q = 16,
    key_w = 17,
    key_e = 18,
    key_r = 19,
    key_t = 20,
    key_y = 21,
    key_u = 22,
    key_i = 23,
    key_o = 24,
    key_p = 25,
    key_left_sq_bracket = 26,
    key_right_sq_bracket = 27,
    key_enter = 28,
    key_left_ctrl = 29,
    key_a = 30,
    key_s = 31,
    key_d = 32,
    key_f = 33,
    key_g = 34,
    key_h = 35,
    key_j = 36,
    key_k = 37,
    key_l = 38,
    key_semicolon = 39,
    key_single_quote = 40,
    key_back_tick = 41,
    key_left_shift = 42,
    key_backslash = 43,
    key_z = 44,
    key_x = 45,
    key_c = 46,
    key_v = 47,
    key_b = 48,
    key_n = 49,
    key_m = 50,
    key_comma = 51,
    key_dot = 52,
    key_slash = 53,
    key_right_shift = 54,
    key_keypad_star = 55,
    key_left_alt = 56,
    key_space = 57,
    key_capslock = 58,
    key_f1 = 59,
    key_f2 = 60,
    key_f3 = 61,
    key_f4 = 62,
    key_f5 = 63,
    key_f6 = 64,
    key_f7 = 65,
    key_f8 = 66,
    key_f9 = 67,
    key_f10 = 68,
    key_numberlock = 69,
    key_scrolllock = 70,
    key_keypad_7 = 71,
    key_keypad_8 = 72,
    key_keypad_9 = 73,
    key_keypad_minus = 74,
    key_keypad_4 = 75,
    key_keypad_5 = 76,
    key_keypad_6 = 77,
    key_keypad_plus = 78,
    key_keypad_1 = 79,
    key_keypad_2 = 80,
    key_keypad_3 = 81,
    key_keypad_0 = 82,
    key_keypad_dot = 83,
    key_f11 = 87,
    key_f12 = 88
} micros_keyboard_keys;

#ifdef __cplusplus
extern "C"
{
#endif

//! Checks if any key is pressed
/*!
    Checks if any key is pressed and returns flag.
    \return True if some key is pressed and is ready to retrieve, otherwise false.
*/
bool micros_keyboard_is_key_pressed();

//! Retrieves pressed key from the buffer if possible
/*!
    Retrieves pressed key from the buffer if possible and fill passed struture with data about it.
    \param scan_ascii_pair Structure which will be filled with data about it.
    \return True if key was available in the buffer and structure was filled, otherwise false.
*/
bool micros_keyboard_get_pressed_key(micros_keyboard_scan_ascii_pair *scan_ascii_pair);

//! Retrieves pressed key from the buffer (or waits if buffer is empty)
/*!
    Retrieves pressed key from the buffer (or waits if buffer is empty) and fill passed struture with data about it.
    \param scan_ascii_pair Structure which will be filled with data about it.
*/
void micros_keyboard_wait_for_key_press(micros_keyboard_scan_ascii_pair *scan_ascii_pair);

//! Retrieves key state (is pressed or not)
/*!
    Retrieves key state (is pressed or not) and returns boolean.
    \param scancode Structure with the key data to check.
    \return True if the key with the specified scancode is pressed, otherwise false.
*/
bool micros_keyboard_get_key_state(micros_keyboard_keys scancode);

#ifdef __cplusplus
}
#endif

#endif