#include "terminal_manager.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"
#include "process/manager/process_manager.h"

uint32_t active_terminal_id = 0;
uint32_t next_terminal_id = 0;
terminal_struct* terminals_array = NULL;
uint32_t terminal_number = 0;

int8_t create_terminal(uint32_t* terminal_id)
{
    void *ptr;
    if(terminals_array == NULL)
        ptr = heap_kernel_alloc(sizeof(terminal_struct), 0);
    else
        ptr = heap_kernel_realloc(terminals_array, (terminal_number + 1) * sizeof(terminal_struct), 0);
    if(ptr == NULL) return -1;
    terminals_array = ptr;
    terminals_array[terminal_number].terminal_id = next_terminal_id;
    terminals_array[terminal_number].cursor_position_x = 0;
    terminals_array[terminal_number].cursor_position_y = 0;
    terminals_array[terminal_number].screen_mode = 0x03;
    terminals_array[terminal_number].screen_buffer = video_card_create_external_buffer(0x03);
    uint32_t process_id = process_manager_create_process("A:/ENV/SHELL.ELF", "", 1000, false);
    terminals_array[terminal_number].active_process = process_manager_get_process_info(process_id);
    process_manager_set_active_process_id(process_id);
    active_terminal_id = next_terminal_id;
    *terminal_id = active_terminal_id;
    terminal_number++;
    next_terminal_id++;
    return 0;
}

int8_t destroy_terminal(uint32_t terminal_id)
{
    bool move = false;
    terminal_struct s;
    for(uint32_t i = 0; i < terminal_number; i++)
    {
        if(terminals_array[i].terminal_id = terminal_id)
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

    if(terminal_number == 1){
        heap_kernel_dealloc(terminals_array);
        terminals_array = NULL;
        active_terminal_id = 0;
    }
    else {
        heap_kernel_realloc(terminals_array, (terminal_number - 1) * sizeof(terminal_struct), 0);
        active_terminal_id = terminals_array[0].terminal_id;
    }
    terminal_number--;
    return 0;
}

int8_t destroy_active_terminal()
{
    return destroy_active_terminal(active_terminal_id);
}

int8_t switch_active_terminal(uint32_t terminal_id)
{
    for(int i = 0; i < terminal_number; i++)
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
    for(int i = 0; i < terminal_number; i++)
    {
        if(terminals_array[i].terminal_id == active_terminal_id){
            if(i+1 == terminal_number)
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

int8_t terminal_manager_set_mode(uint32_t terminal_id, int8_t mode)
{
    for(uint32_t i; i < terminal_number; i++)
    {
        if(terminals_array[i].terminal_id == terminal_id)
        {
            video_card_destroy_external_buffer(terminals_array[i].screen_buffer);
            terminals_array[i].screen_buffer = video_card_create_external_buffer(mode);
            terminals_array[i].screen_mode = mode;
            video_card_clear_screen_external_buffer(terminals_array[i].screen_buffer, mode);

            if(terminals_array[i].terminal_id == active_terminal_id)
            {
                video_card_set_video_mode(mode);
                video_card_clear_screen();
            }
            return mode;
        }
    }
    return -1;
}

int8_t terminal_manager_print_char(uint32_t terminal_id, char character);
int8_t terminal_manager_print_char_color(uint32_t terminal_id, char character, uint8_t color);
int8_t terminal_manager_print_string(uint32_t terminal_id, const char* string);
int8_t terminal_manager_print_string_color(uint32_t terminal_id, const char* string, uint8_t color);
int8_t terminal_manager_set_char(uint32_t terminal_id, uint16_t x, uint16_t y, char character);
int8_t terminal_manager_get_char(uint32_t terminal_id, uint16_t x, uint16_t y, char* character);
int8_t terminal_manager_set_color(uint32_t terminal_id, uint16_t x, uint16_t y, uint8_t color);
int8_t terminal_manager_get_color(uint32_t terminal_id, uint16_t x, uint16_t y, uint8_t* color);
int8_t terminal_manager_set_char_and_color(uint32_t terminal_id, uint16_t x, uint16_t y, char character, uint8_t color);
int8_t terminal_manager_get_char_and_color(uint32_t terminal_id, uint16_t x, uint16_t y, char* character, uint8_t* color);
int8_t terminal_manager_set_cursor_pos(uint32_t terminal_id, uint16_t x, uint16_t y);
int8_t terminal_manager_get_cursor_pos(uint32_t terminal_id, uint16_t* x, uint16_t* y);
int8_t terminal_manager_turn_cursor_on(uint32_t terminal_id);
int8_t terminal_manager_turn_cursor_off(uint32_t terminal_id);

int8_t terminal_manager_draw_pixel(uint32_t terminal_id, uint8_t color, uint16_t x, uint16_t y);
int8_t terminal_manager_draw_line(uint32_t terminal_id, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t terminal_manager_draw_circle(uint32_t terminal_id, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t terminal_manager_draw_rectangle(uint32_t terminal_id, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t terminal_manager_clear_screen(uint32_t terminal_id);

//TODO: Functions for writting to chars and pixels, changing modes, changing active terminal and get list of terminals