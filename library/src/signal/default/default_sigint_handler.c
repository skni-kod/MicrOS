#include "../../signal.h"

void default_sigint_handler(int param)
{
    fprintf(stderr, "SIGINT received.\n", param);
    exit(0);
}