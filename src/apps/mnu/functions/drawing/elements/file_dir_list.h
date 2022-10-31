#include "misc/file_list.h"

// this one displays a list of elements from a given list from given index and a given amount of elements
void mnu_draw_file_dir_list(mnu_file_list* list_pointer, unsigned short int first, unsigned short int last, unsigned short int cursor_position){

    for (unsigned short int n = first; n <= last; n++){

        // colors to be changed

        if (list_pointer->are_they_dirs[n] == true){
            // dir info
            micros_console_set_foreground_color(micros_console_color_white);
            printf("<");
            micros_console_set_foreground_color(micros_console_color_cyan);
            printf("dir ");
            micros_console_set_foreground_color(micros_console_color_white);
            printf("> ");

        }
        else{
            
            // file info
            micros_console_set_foreground_color(micros_console_color_white);
            printf("<");
            micros_console_set_foreground_color(micros_console_color_yellow);
            printf("file");
            micros_console_set_foreground_color(micros_console_color_white);
            printf("> ");

        }

        if (cursor_position == n){
            // draws the cursor
            micros_console_set_foreground_color(micros_console_color_light_green);
            printf("-> ");
        }

        micros_console_set_foreground_color(micros_console_color_light_gray);
        printf("%s\n", list_pointer->names[n]);
    }
}