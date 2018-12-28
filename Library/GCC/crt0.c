#include "crt0.h"

void _start(int argc, char *argv[])
{
    exit(main(argc, argv));
}