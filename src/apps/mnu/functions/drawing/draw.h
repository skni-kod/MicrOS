#include "elements/top_bar.h"
#include "elements/current_dir.h"
#include "elements/file_dir_list.h"

void mnu_draw_screen_main(mnu_file_list* list_pointer, unsigned short int first, unsigned short int last, unsigned short int cursor_position){

    mnu_draw_top_bar();
    mnu_draw_current_dir();
    mnu_draw_file_dir_list(list_pointer, first, last, cursor_position);
    
}