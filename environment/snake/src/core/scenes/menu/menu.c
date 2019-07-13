#include "menu.h"

menu_button selected_menu_button;
point indicator_start_position;

bool redraw_menu;
bool button_selected;

const char *main_menu_screen =
"\n"
"\n"
"\n"
"               _____                               _               \n"
"              / ____|                             | |              \n"
"             | (___        _ __         __ _      | | __       ___ \n"
"              \\___ \\      | '_ \\       / _` |     | |/ /      / _ \\\n"
"              ____) |     | | | |     | (_| |     |   <      |  __/\n"
"             |_____/      |_| |_|      \\__,_|     |_|\\_\\      \\___|\n"
"\n"
"\n"
"\n"
"                                   Play\n"
"                                   Highscores\n"
"                                   About\n"
"                                   Exit\n";

void menu_init()
{
    micros_console_clear();
    
    if(indicator_start_position.x == 0 && indicator_start_position.y == 0)
    {
        selected_menu_button = menu_button_play;
        indicator_start_position.x = 33;
        indicator_start_position.y = 12;
    }
    
    redraw_menu = true;
    button_selected = false;
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
                button_selected = true;
                break;
            }
        } 
    }
}

scene_type menu_logic()
{
    if(button_selected)
    {
        switch(selected_menu_button)
        {
            case menu_button_play: return scene_type_game;
            case menu_button_highscores: return scene_type_highscores;
            case menu_button_about: return scene_type_about;
            case menu_button_exit: return scene_type_exit;
        }    
    }
    
    return scene_type_none;
}

void menu_draw()
{
    if(redraw_menu)
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
        
        redraw_menu = false;
    }
}

void menu_delete()
{
    
}