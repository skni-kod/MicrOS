#include "terminal_manager.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"
#include "process/manager/process_manager.h"

uint32_t active_terminal_id = 0;
uint32_t next_terminal_id = 0;
terminal_struct* terminals_array = NULL;
uint32_t _terminal_number = 0;

terminal_struct* find_terminal_for_process(uint32_t process_id)
{
    for(uint32_t i=0; i<_terminal_number; i++)
    {
        if(terminals_array[i].active_process->id == process_id)
        {
            return terminals_array + i;
        }
    }
    return NULL;
}

const terminal_struct* get_terminals(uint32_t* terminal_number)
{
    *terminal_number = _terminal_number;
    return terminals_array;
}

void init_terminal_manager()
{
    
}

int8_t create_terminal(uint32_t* terminal_id)
{
    void *ptr;
    if(terminals_array == NULL)
        ptr = heap_kernel_alloc(sizeof(terminal_struct), 0);
    else
        ptr = heap_kernel_realloc(terminals_array, (_terminal_number + 1) * sizeof(terminal_struct), 0);
    if(ptr == NULL) return -1;
    terminals_array = ptr;
    terminals_array[_terminal_number].terminal_id = next_terminal_id;
    terminals_array[_terminal_number].cursor_position_x = 0;
    terminals_array[_terminal_number].cursor_position_y = 0;
    terminals_array[_terminal_number].screen_mode = 0x03;
    terminals_array[_terminal_number].screen_buffer = video_card_create_external_buffer(0x03);
    uint32_t process_id = process_manager_create_process("A:/ENV/SHELL.ELF", "", 1000, false);
    terminals_array[_terminal_number].active_process = process_manager_get_process_info(process_id);
    process_manager_set_active_process_id(process_id);
    active_terminal_id = next_terminal_id;
    *terminal_id = active_terminal_id;
    _terminal_number++;
    next_terminal_id++;
    return 0;
}

int8_t destroy_terminal(uint32_t terminal_id)
{
    bool move = false;
    terminal_struct s;
    for(uint32_t i = 0; i < _terminal_number; i++)
    {
        if(terminals_array[i].terminal_id == terminal_id)
        {
            move = true;
            s = terminals_array[i];
        }
        if(move)
            terminals_array[i-1] = terminals_array[i];
    }

    process_info* p = s.active_process;
    process_info* parrent_p;
    while(p->parent_id != 1000)
    {
        parrent_p = process_manager_get_process_info(p->parent_id);
        process_manager_close_process(p->parent_id);
        p = parrent_p;
    }

    if(_terminal_number == 1){
        heap_kernel_dealloc(terminals_array);
        terminals_array = NULL;
        active_terminal_id = 0;
    }
    else {
        heap_kernel_realloc(terminals_array, (_terminal_number - 1) * sizeof(terminal_struct), 0);
        active_terminal_id = terminals_array[0].terminal_id;
    }
    _terminal_number--;
    return 0;
}

int8_t destroy_active_terminal()
{
    return destroy_active_terminal(active_terminal_id);
}

int8_t switch_active_terminal(uint32_t terminal_id)
{
    for(uint32_t i = 0; i < _terminal_number; i++)
    {
        if(terminals_array[i].terminal_id == terminal_id){
            active_terminal_id = terminal_id;
            video_card_set_video_mode(terminals_array[i].screen_mode);
            video_card_swap_external_buffer(terminals_array[i].screen_buffer, terminals_array[i].screen_mode);
            return 0;
        }
    }
    return -1;
}

int8_t next_terminal()
{
    for(uint32_t i = 0; i < _terminal_number; i++)
    {
        if(terminals_array[i].terminal_id == active_terminal_id){
            if(i+1 == _terminal_number)
            {
                active_terminal_id = terminals_array[0].terminal_id;
                video_card_set_video_mode(terminals_array[0].screen_mode);
                video_card_swap_external_buffer(terminals_array[0].screen_buffer, terminals_array[i].screen_mode);
            }
            else{
                active_terminal_id = terminals_array[i+1].terminal_id;
                video_card_set_video_mode(terminals_array[i+1].screen_mode);
                video_card_swap_external_buffer(terminals_array[i+1].screen_buffer, terminals_array[i].screen_mode);
            }
            return 0;
        }
    }
    return -1;
}

int8_t terminal_manager_set_mode(uint32_t process_id, int8_t mode)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    uint8_t* buffer = video_card_create_external_buffer(mode);
    if(buffer == NULL) return -1;
    video_card_destroy_external_buffer(terminal->screen_buffer);
    terminal->screen_buffer = buffer;
    terminal->screen_mode = mode;
    video_card_clear_screen_external_buffer(buffer, mode);
    if(terminal->terminal_id == active_terminal_id)
    {
        video_card_set_video_mode(mode);
        video_card_clear_screen();
    }
    return mode;
}

int8_t terminal_manager_print_char(uint32_t process_id, char character)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_print_char_external_buffer(
        terminal->screen_buffer, 
        terminal->screen_mode, 
        &(terminal->cursor_position_x), 
        &(terminal->cursor_position_y), 
        character);
    if(terminal->terminal_id == active_terminal_id) video_card_print_char(character);
    return 0;
}

int8_t terminal_manager_print_char_color(uint32_t process_id, char character, uint8_t color)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_print_char_color_external_buffer(
        terminal->screen_buffer, 
        terminal->screen_mode, 
        &(terminal->cursor_position_x), 
        &(terminal->cursor_position_y), 
        character, 
        color);
    if(terminal->terminal_id == active_terminal_id) video_card_print_char_color(character, color);
    return 0;
}

int8_t terminal_manager_print_string(uint32_t process_id, const char* string)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_print_string_external_buffer(
        terminal->screen_buffer, 
        terminal->screen_mode, 
        &(terminal->cursor_position_x), 
        &(terminal->cursor_position_y), 
        string);
    if(terminal->terminal_id == active_terminal_id) video_card_print_string(string);
    return 0;
}

int8_t terminal_manager_print_string_color(uint32_t process_id, const char* string, uint8_t color)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_print_string_color_external_buffer(
        terminal->screen_buffer, 
        terminal->screen_mode, 
        &(terminal->cursor_position_x), 
        &(terminal->cursor_position_y), 
        string,
        color);
    if(terminal->terminal_id == active_terminal_id) video_card_print_string_color(string, color);
    return 0;
}
int8_t terminal_manager_set_char(uint32_t process_id, uint16_t x, uint16_t y, char character)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_set_char_external_buffer(
        terminal->screen_buffer, 
        terminal->screen_mode, x, y, character);
    if(terminal->terminal_id == active_terminal_id) video_card_set_char(x, y, character);
    return 0;
}

int8_t terminal_manager_get_char(uint32_t process_id, uint16_t x, uint16_t y, char* character)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_get_char_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        x, y, character
    );
    if(terminal->terminal_id == active_terminal_id) video_card_get_char(x, y, character);
    return 0;
}

int8_t terminal_manager_set_color(uint32_t process_id, uint16_t x, uint16_t y, uint8_t color)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_set_color_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        x, y, color
    );
    if(terminal->terminal_id == active_terminal_id) video_card_set_color(x, y, color);
    return 0;
}

int8_t terminal_manager_get_color(uint32_t process_id, uint16_t x, uint16_t y, uint8_t* color)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_get_color_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        x, y, color
    );
    if(terminal->terminal_id == active_terminal_id) video_card_get_color(x, y, color);
    return 0;
}

int8_t terminal_manager_set_char_and_color(uint32_t process_id, uint16_t x, uint16_t y, char character, uint8_t color)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_set_char_and_color_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        x, y, character, color
    );
    return 0;
}

int8_t terminal_manager_get_char_and_color(uint32_t process_id, uint16_t x, uint16_t y, char* character, uint8_t* color)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_get_char_and_color_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        x, y, character, color
    );
    if(terminal->terminal_id == active_terminal_id) video_card_get_char_and_color(x, y, character, color);
    return 0;
}

int8_t terminal_manager_set_cursor_pos(uint32_t process_id, uint16_t x, uint16_t y)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    terminal->cursor_position_x = x;
    terminal->cursor_position_y = y;
    if(terminal->terminal_id == active_terminal_id) video_card_set_cursor_pos(x, y);
    return 0; 
}

int8_t terminal_manager_get_cursor_pos(uint32_t process_id, uint16_t* x, uint16_t* y)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    *x = terminal->cursor_position_x;
    *y = terminal->cursor_position_y;
    if(terminal->terminal_id == active_terminal_id) video_card_get_cursor_pos(x, y);
    return 0;
}

int8_t terminal_manager_turn_cursor_on(uint32_t process_id)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    terminal->cursor = true;
    if(terminal->terminal_id == active_terminal_id) video_card_turn_cursor_on();
    return 0;
}
int8_t terminal_manager_turn_cursor_off(uint32_t process_id)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    terminal->cursor = false;
    if(terminal->terminal_id == active_terminal_id) video_card_turn_cursor_off();
    return 0;
}

int8_t terminal_manager_draw_pixel(uint32_t process_id, uint8_t color, uint16_t x, uint16_t y)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_draw_pixel_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        color, x, y
    );
    if(terminal->terminal_id == active_terminal_id) video_card_draw_pixel(color, x, y);
    return 0;
}

int8_t terminal_manager_draw_line(uint32_t process_id, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_draw_line_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        color, ax, ay, bx, by
    );
    if(terminal->terminal_id == active_terminal_id) video_card_draw_line(color, ax, ay, bx, by);
    return 0;
}

int8_t terminal_manager_draw_circle(uint32_t process_id, uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_draw_circle_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        color, x, y, radius
    );
    if(terminal->terminal_id == active_terminal_id) video_card_draw_circle(color, x, y, radius); 
    return 0;
}

int8_t terminal_manager_draw_rectangle(uint32_t process_id, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_draw_rectangle_external_buffer(
        terminal->screen_buffer,
        terminal->screen_mode,
        color, ax, ay, bx, by
    );
    if(terminal->terminal_id == active_terminal_id) video_card_draw_rectangle(color, ax, ay, bx, by);
    return 0;
}

int8_t terminal_manager_clear_screen(uint32_t process_id)
{
    terminal_struct* terminal = find_terminal_for_process(process_id);
    if(terminal == NULL) return -1;
    video_card_clear_screen_external_buffer(terminal->screen_buffer, 
    terminal->screen_mode);
    if(terminal->terminal_id == active_terminal_id) video_card_clear_screen();
    return 0;
}

//TODO: Functions for writting to chars and pixels, changing modes, changing active terminal and get list of terminals