#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SPACE");
    int free_space = micros_filesystem_get_free_space() / 1000;
    int total_space = micros_filesystem_get_total_space() / 1000;
    int percent = 100 - free_space * 100 / total_space;
    
    printf("Free space: %d KB, Total space: %d KB\n", free_space, total_space);
    printf("Usage: %d %%\n", percent);
    
    return 0;
}