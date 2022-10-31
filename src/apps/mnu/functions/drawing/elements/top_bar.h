#include <micros.h>
#include <stdio.h>


// this draws the bar that is on top of the screen when mnu is on
void mnu_draw_top_bar(){

    micros_console_set_background_color(micros_console_color_white);
    micros_console_set_foreground_color(micros_console_color_black);

    printf("Welcome to MicrOS Navigation Utility, for help press f1\n");

}