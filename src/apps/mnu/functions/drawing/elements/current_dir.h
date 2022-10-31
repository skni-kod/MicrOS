
#include <micros.h>
#include <stdio.h>
#include "misc/file_list.h"

// this draws current cirectory information
void mnu_draw_current_dir(){

    micros_console_set_background_color(micros_console_color_black);
    micros_console_set_foreground_color(micros_console_color_white);

    printf("Current dir: ");

    micros_console_set_foreground_color(micros_console_color_cyan);
    printf("%s\n", path_main);
}