#include "../../signal.h"

void default_sigsegv_handler(int param)
{
    micros_console_set_video_mode(0x03);
    fprintf(stderr, "SIGSEGV received. Address: %#x\n", param);
    micros_process_exit_process(-1);
}