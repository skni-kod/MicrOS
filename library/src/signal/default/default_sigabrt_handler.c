#include "../../signal.h"

void default_sigabrt_handler(int param)
{
    fprintf(stderr, "SIGABRT received.\n", param);
    abort();
}