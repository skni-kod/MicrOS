#include "../../signal.h"

void default_sigterm_handler(int param)
{
    fprintf(stderr, "SIGTERM received.\n", param);
    exit(0);
}