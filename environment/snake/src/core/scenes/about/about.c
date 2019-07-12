#include "about.h"

bool redraw_about;

const char *about_screen =
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
"                                                      for MicrOS <3\n"
"\n"
"\n"
"\n"
"                                  Version 1.0\n"
"                                   SKNI  KOD \n"
"\n"
"                            Press any key to return\n";

void about_init()
{
    micros_console_clear();
    redraw_about = true;
}

void about_input()
{
    
}

scene_type about_logic()
{
    if(!redraw_about)
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_wait_for_key_press(&pressed_key);
        
        return scene_type_main_menu;
    }
    
    return scene_type_none;
}

void about_draw()
{
    if(redraw_about)
    {
        micros_console_position cursor_position;
        
        cursor_position.x = 0;
        cursor_position.y = 0;
        micros_console_set_cursor_position(&cursor_position);
        
        micros_console_set_background_color(micros_console_color_black);
        micros_console_set_foreground_color(micros_console_color_light_gray);
        
        printf(about_screen);
            
        redraw_about = false;
    }
}

void about_delete()
{
    
}