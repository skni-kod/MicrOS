#include "../../signal.h"

void default_sigsegv_handler(int param)
{
    // TODO: Replace with printf and add param
    micros_console_print_string("SIGSEGV received. Address:\n");
    micros_process_exit(-1);
}