// this file contains the main loop of the file manager
#include "drawing/draw.h"
#include <micros/sys/micros_keyboard.h>

// where is the cursor
unsigned short int cursor_position = 0;
// file list
mnu_file_list main_file_list;
// for going a dir back
char dir_depth_level = 0;
// from where to display files
unsigned short int lower_limit = 0;
// to where to display files
unsigned short int upper_limit = 0;

// moves cursor up or loops
void mnu_move_file_cursor_up()
{

    if (cursor_position > 0)
    {
        cursor_position--;
        return;
    }

    cursor_position = main_file_list.length - 1;
}

void mnu_move_file_cursor_down()
{

    if (cursor_position < main_file_list.length - 1)
    {
        cursor_position++;
        return;
    }

    cursor_position = 0;
}

void mnu_advance_directory(mnu_file_list *list_pointer)
{

    // this function enters a new dir

    // allocates memory for new path to the file / directory
    char *new_path = malloc(strlen(list_pointer->names[cursor_position]) + strlen(path_main) + 1);

    strcpy(new_path, path_main);
    strcat(new_path, list_pointer->names[cursor_position]);

    // causes paging error, might do realloc and copy new_path to it or something                                                   !URGENT! !MEMORY LEAK!
    free(path_main);

    path_main = new_path;

    mnu_file_list_fill(list_pointer);

    upper_limit = main_file_list.length - 1;

    // to not draw too much to preserve performance
    if (upper_limit > 22)
    {
        upper_limit = 22;
    }

    lower_limit = 0;
    dir_depth_level++;
    cursor_position = 0;
}

// will be done later, time is short
// might be done as a function searching for slashes or a list of pervious folders, or both as a dir before and back function
void mnu_unadvance_directory(mnu_file_list *list_pointer, char *argv[])
{

    // this one goes back a dir
    // if (dir_depth_level > 0){

    // searches for last slash and reallocs memory to cut off the rest
    short int last_slash_index = strlen(path_main) - 2;
    for (; last_slash_index > -1; last_slash_index--)
    {
        if (path_main[last_slash_index] == '/')
        {
            break;
        }
    }

    // switch this realloc with the next malloc section to get the state that works

    // realloc
    realloc(path_main, last_slash_index + 2);
    path_main[last_slash_index + 1] = '\0';

    // proper workaround around broken realloc

    /*char* temp_string = malloc(strlen(path_main) + 1);
    strcpy(temp_string, path_main);
    free(path_main);
    path_main = malloc( last_slash_index + 2);
    for (unsigned short int n = 0; n < last_slash_index + 1; n++){
        path_main[n] = temp_string[n];
    }
    path_main[last_slash_index + 1] = '\0';
    free(temp_string);*/

    // end of malloc thingy

    // last character
    path_main[strlen(argv[1])] = 0;

    mnu_file_list_fill(list_pointer);

    upper_limit = main_file_list.length - 1;

    // to not draw too much to preserve performance
    if (upper_limit > 22)
    {
        upper_limit = 22;
    }

    lower_limit = 0;
    dir_depth_level--;
    cursor_position = 0;

    //}
}

// main path is the starting dir, with high possibility "A:/"
void mnu_main_loop(char *argv[])
{

    // some preparation stuff here

    // preparing the files for user

    main_file_list.length = 0;
    mnu_file_list_fill(&main_file_list);

    // variables to control the program

    upper_limit = main_file_list.length - 1;

    // to not draw too much to preserve performance
    if (upper_limit > 22)
    {
        upper_limit = 22;
    }
    // current stage variable
    char current_stage_number = 0;
    /*
    0 - main screen
    1 - help screen
    */

    bool keep_working = 1;

    // doing things
    while (keep_working)
    {

        // uncomment to erase debug data
        // micros_console_clear();

        // stages are just screens of the program

        switch (current_stage_number)
        {
        case 0:
            mnu_draw_screen_main(&main_file_list, lower_limit, upper_limit, cursor_position);
            break;
        case 1:
            // help screen
            break;
        }

        // key handling
        do
        {
            micros_keyboard_scan_ascii_pair key_pair;

            micros_keyboard_wait_for_key_press(&key_pair);

            switch (key_pair.scancode)
            {

            case key_esc:

                keep_working = 0;
                break;

            // up key
            case key_keypad_8:
                mnu_move_file_cursor_up();
                break;

            // down
            case key_keypad_2:
                mnu_move_file_cursor_down();
                break;

            // enter, enters a new dir or opens a file (?)
            case key_enter:

                // only dirs for now
                if (main_file_list.are_they_dirs[cursor_position] == 1)
                {
                    mnu_advance_directory(&main_file_list);
                }

                break;

            // going back a dir
            case key_backspace:

                mnu_unadvance_directory(&main_file_list, argv);

                printf("%s\n", path_main);

                break;

            default:
                continue;
            }
        } while (0);
    }

    mnu_file_list_free(&main_file_list);

    // clearing the console after use
    micros_console_clear();
}