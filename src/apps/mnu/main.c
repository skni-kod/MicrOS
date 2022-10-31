// This is planned to be a vga mode file manager for MicrOS
// the base sturcture is inspired by the snake game, by that I mean that I learned how to make it launch and display from it

#include "functions/loop.h"

// vga mode had 25 rows and 80 columns

// main function of the program
int main(int argc, char *argv[])
{

    // current_partition = argv[1][0];
    micros_process_set_current_process_name("MNU");
    micros_console_set_cursor_visibility(false);

    // current path

    // current dir moved to malloc so that it can be freed
    path_main = malloc(strlen(argv[1]) + 1);
    for (unsigned short int n = 0; n < strlen(argv[1]); n++)
    {
        path_main[n] = argv[1][n];
    }
    // last character
    path_main[strlen(argv[1])] = 0;

    mnu_main_loop(argv);

    return 0;
}