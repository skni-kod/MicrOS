#include "highscores.h"

void highscores_init()
{
    micros_console_clear();
    
    micros_console_position cursor_position;
    
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    printf("NOT IMPLEMENTED YET");
}

void highscores_input()
{
    
}

scene_type highscores_logic()
{
    return scene_type_none;
}

void highscores_draw()
{
    
}

void highscores_delete()
{
    
}