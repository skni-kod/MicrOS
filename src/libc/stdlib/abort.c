#include <stdlib.h>

void abort()
{
    micros_process_exit_process(-1);
}

#ifndef __libk
#pragma message "libc"
#else
 #pragma message "libk"
#endif