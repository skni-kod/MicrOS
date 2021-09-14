#include "core/core.h"
#include <micros/micros_console.h>

char current_partition;

char dupa[320*200];

int main(int argc, char *argv[])
{
    current_partition = argv[1][0];
    //core_run();
    for(int i = 0 ; i < 320 * 200; i++)
    {
        dupa[i] = 40;
    }
    micros_console_set_video_mode(0x13);
    micros_console_copy_from_buffer(dupa, 320*200);
    while(1);
    return 0;
}