#include "../../signal.h"

void default_sigsegv_handler(int param)
{
    micros_console_print_string("SIGSEGV received.\n");
    micros_process_exit(-1);
}