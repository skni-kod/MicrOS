#include "highscores.h"

highscores_container highscores_data;
point list_start_point;

bool exit_to_menu;

const char *highscores_filename = "/ENV/SN_HGHSC.SAV";
const char *highscores_screen =
"\n"
"\n"
"\n"
"                 _    _ _       _                                \n"
"                | |  | (_)     | |                               \n"
"                | |__| |_  __ _| |__  ___  ___ ___  _ __ ___ ___ \n"
"                |  __  | |/ _` | '_ \\/ __|/ __/ _ \\| '__/ _ / __|\n"
"                | |  | | | (_| | | | \\__ | (_| (_) | | |  __\\__ \\\n"
"                |_|  |_|_|\\__, |_| |_|___/\\___\\___/|_|  \\___|___/\n"
"                           __/ |                                 \n"
"                          |___/                                  \n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"                              Press ENTER to return\n";

void highscores_init()
{
    micros_console_clear();
    
    list_start_point.x = 13;
    list_start_point.y = 13;
    exit_to_menu = false;
    
    micros_console_position cursor_position;
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    printf(highscores_screen);
    
    for(int i = 0; i < HIGHSCORES_ENTRIES_COUNT; i++)
    {
        if(highscores_data.entries[i].name[0] != 0)
        {
            cursor_position.x = list_start_point.x;
            cursor_position.y = list_start_point.y + i;
            micros_console_set_cursor_position(&cursor_position);
            
            printf("%d. %s: %d points", i, highscores_data.entries[i].name, highscores_data.entries[i].score);
        }
    }
}

void highscores_input()
{
    if(micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);
        
        if (pressed_key.scancode == key_enter)
        {
            exit_to_menu = true;
        } 
    }
}

scene_type highscores_logic()
{
    return exit_to_menu ? scene_type_main_menu : scene_type_none;
}

void highscores_draw()
{
    
}

void highscores_delete()
{
    
}

void highscores_load_from_file()
{
    if(micros_filesystem_file_exists(highscores_filename))
    {
        FILE *file = fopen(highscores_filename, "r");
        fread(&highscores_data, sizeof(highscores_container), 1, file);
        fclose(file);
    }
}

void highscores_save_to_file()
{
    FILE *file = fopen(highscores_filename, "w");
    fwrite(&highscores_data, sizeof(highscores_container), 1, file);
    fclose(file);
}