#include "menu.h"

menu_button selected_menu_button;
point indicator_start_position;
game_state requested_game_state;

bool redraw_menu;

void menu_init()
{
    micros_console_clear();
    
    selected_menu_button = menu_button_play;
    indicator_start_position.x = 33;
    indicator_start_position.y = 12;
    
    redraw_menu = true;
}

void menu_input()
{
    if(micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);
        
        switch(pressed_key.scancode)
        {
            case key_keypad_8:
            {
                selected_menu_button = (selected_menu_button - 1) % 4;
                redraw_menu = true;
                break;
            }
            
            case key_keypad_2:
            {
                selected_menu_button = (selected_menu_button + 1) % 4;
                redraw_menu = true;
                break;
            }
            
            case key_enter:
            {
                menu_set_requested_state();
                break;
            }
        } 
    }
}

bool menu_logic()
{
    if(redraw_menu)
    {
        redraw_menu = false;
        return true;
    }
    
    return false;
}

void menu_draw()
{
    micros_console_position cursor_position;
    
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    printf(main_menu_screen);
    
    for(int i = 0; i < 4; i++)
    {
        cursor_position.x = indicator_start_position.x;
        cursor_position.y = indicator_start_position.y + i;
        micros_console_set_cursor_position(&cursor_position);
        
        printf(" ");
    }
    
    cursor_position.x = indicator_start_position.x;
    cursor_position.y = indicator_start_position.y + selected_menu_button;
    micros_console_set_cursor_position(&cursor_position);
    
    printf(">");
}

void menu_set_requested_state()
{
    switch(selected_menu_button)
    {
        case menu_button_play:
        {
            requested_game_state = game_state_pregame;
            break;
        }
    }
}

game_state menu_get_requested_state()
{
    game_state state_to_return = requested_game_state;
    requested_game_state = 0;
    
    return state_to_return;
}