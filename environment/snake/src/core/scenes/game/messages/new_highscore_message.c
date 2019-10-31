#include "new_highscore_message.h"

point input_position;

const char *new_highscore_message_screen =
"\n"
"\n"
"\n"
"\n"
"\n"
"       _   _                 _     _       _                            _ \n"
"      | \\ | |               | |   (_)     | |                          | |\n"
"      |  \\| | _____      __ | |__  _  __ _| |__  ___  ___ ___  _ __ ___| |\n"
"      | . ` |/ _ \\ \\ /\\ / / | '_ \\| |/ _` | '_ \\/ __|/ __/ _ \\| '__/ _ | |\n"
"      | |\\  |  __/\\ V  V /  | | | | | (_| | | | \\__ | (_| (_) | | |  __|_|\n"
"      |_| \\_|\\___| \\_/\\_/   |_| |_|_|\\__, |_| |_|___/\\___\\___/|_|  \\___(_)\n"
"                                      __/ |                               \n"
"                                     |___/                                \n"
"\n"
"\n"
"                                  Your score: %d\n"
"                        Enter your name and press ENTER:\n";

void new_highscore_message_draw_and_wait(int score)
{
    input_position.x = 24;
    input_position.y = 17;
    
    micros_console_clear();
    
    micros_console_position cursor_position;
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    micros_console_set_background_color(micros_console_color_black);
    micros_console_set_foreground_color(micros_console_color_light_gray);
    
    printf(new_highscore_message_screen, score);
    
    char name[HIGHSCORES_NAME_LENGTH];
    do
    {
        cursor_position.x = input_position.x;
        cursor_position.y = input_position.y;
        micros_console_set_cursor_position(&cursor_position);
    
        fgets(name, HIGHSCORES_NAME_LENGTH, stdin);
    }
    while(name[0] == '\n');
    
    for(size_t i = 0; i < strlen(name); i++)
    {
        if(name[i] == '\n')
        {
            name[i] = 0;
            break;
        }
    }
    
    highscores_add_new_entry(name, score);
}