#ifndef TERMINAL_MANAGER_H
#define TERMINAL_MANAGER_H

#include <stdint.h>
#include "../process/manager/process_info.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/keyboard/keyboard_state_flags.h"

typedef struct terminal_struct
{
    bool cursor;
    uint32_t terminal_id;
    int8_t screen_mode;
    uint16_t cursor_position_x;
    uint16_t cursor_position_y;
    uint8_t* screen_buffer;
    process_info* active_process;
    process_info** attached_processes;
    uint32_t process_number;

} terminal_struct;

void init_terminal_manager();

int8_t attach_process_to_terminal(uint32_t terminal_id, process_info* p_info);
int8_t dettached_process_from_terminal(process_info* p_info);

terminal_struct* find_terminal_for_process(uint32_t process_id);
const terminal_struct* get_terminals();

int8_t create_terminal(uint32_t* terminal_id);
int8_t destroy_terminal(uint32_t terminal_id);
int8_t destroy_active_terminal();

int8_t switch_active_terminal(uint32_t terminal_id);
int8_t next_terminal();

int8_t terminal_manager_set_mode(uint32_t process_id, int8_t mode);
int8_t terminal_manager_print_char(uint32_t process_id, char character);
int8_t terminal_manager_print_char_color(uint32_t process_id, char character, uint8_t color);
int8_t terminal_manager_print_string(uint32_t process_id, const char* string);
int8_t terminal_manager_print_string_color(uint32_t process_id, const char* string, uint8_t color);
int8_t terminal_manager_set_char(uint32_t process_id, uint16_t x, uint16_t y, char character);
int8_t terminal_manager_get_char(uint32_t process_id, uint16_t x, uint16_t y, char* character);
int8_t terminal_manager_set_color(uint32_t process_id, uint16_t x, uint16_t y, uint8_t color);
int8_t terminal_manager_get_color(uint32_t process_id, uint16_t x, uint16_t y, uint8_t* color);
int8_t terminal_manager_set_char_and_color(uint32_t process_id, uint16_t x, uint16_t y, char character, uint8_t color);
int8_t terminal_manager_get_char_and_color(uint32_t process_id, uint16_t x, uint16_t y, char* character, uint8_t* color);
int8_t terminal_manager_set_cursor_pos(uint32_t process_id, uint16_t x, uint16_t y);
int8_t terminal_manager_get_cursor_pos(uint32_t process_id, uint16_t* x, uint16_t* y);
int8_t terminal_manager_turn_cursor_on(uint32_t process_id);
int8_t terminal_manager_turn_cursor_off(uint32_t process_id);

int8_t terminal_manager_draw_pixel(uint32_t process_id, uint8_t color, uint16_t x, uint16_t y);
int8_t terminal_manager_draw_line(uint32_t process_id, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t terminal_manager_draw_circle(uint32_t process_id, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t terminal_manager_draw_rectangle(uint32_t process_id, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t terminal_manager_clear_screen(uint32_t process_id);
int8_t terminal_manager_copy_from_buffer(uint32_t process_id, uint8_t* buffer, size_t how_many);

uint32_t terminal_manager_get_active_terminal_id();
bool terminal_manager_keyboard_interrupt_handler(interrupt_state *state);

#endif