#include "highscores.h"

highscores_container highscores_data;
point list_start_point;

bool highscores_exit_to_menu;

extern char current_partition;

const char *highscores_path= "/DATA/SNAKE.SAV";
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
"\n"
"\n"
"                              Press ENTER to return\n";

void highscores_init()
{
    micros_console_clear();
    
    list_start_point.x = 16;
    list_start_point.y = 12;
    highscores_exit_to_menu = false;
    
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
            
            printf("%d. %s: %d points", i + 1, highscores_data.entries[i].name, highscores_data.entries[i].score);
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
            highscores_exit_to_menu = true;
        } 
    }
}

scene_type highscores_logic()
{
    return highscores_exit_to_menu ? scene_type_main_menu : scene_type_none;
}

void highscores_draw()
{
    
}

void highscores_delete()
{
    
}

void highscores_load_from_file()
{
    char highscores_path_with_partition[64];
    sprintf(highscores_path_with_partition, "%c:%s", current_partition, highscores_path);
    
    if(micros_filesystem_file_exists(highscores_path_with_partition))
    {
        FILE *file = fopen(highscores_path_with_partition, "r");
        fread(&highscores_data, sizeof(highscores_container), 1, file);
        fclose(file);
    }
}

void highscores_save_to_file()
{
    char highscores_path_with_partition[64];
    sprintf(highscores_path_with_partition, "%c:%s", current_partition, highscores_path);
    
    FILE *file = fopen(highscores_path_with_partition, "w");
    fwrite(&highscores_data, sizeof(highscores_container), 1, file);
    fclose(file);
}

int highscores_get_minimal_score_to_save()
{
    int minimal_score = INT32_MAX;
    for(int i = 0; i < HIGHSCORES_ENTRIES_COUNT; i++)
    {
        if(highscores_data.entries[i].score < minimal_score)
        {
            minimal_score = highscores_data.entries[i].score;
        }
    }
    
    return minimal_score;
}

void highscores_add_new_entry(const char *name, int score)
{
    for(int i = 0; i < HIGHSCORES_ENTRIES_COUNT; i++)
    {
        if(score >= highscores_data.entries[i].score)
        {
            for(int shift_index = HIGHSCORES_ENTRIES_COUNT - 1; shift_index > i; shift_index--)
            {
                memcpy(&highscores_data.entries[shift_index], &highscores_data.entries[shift_index - 1], sizeof(highscores_entry));
            }
            
            memcpy(highscores_data.entries[i].name, name, strlen(name) + 1);
            highscores_data.entries[i].score = score;
            
            break;
        }
    }
    
    highscores_save_to_file();
}