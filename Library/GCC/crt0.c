#include "crt0.h"

void _start(char *path, char *parameters)
{
    exit(main(0, NULL));
}