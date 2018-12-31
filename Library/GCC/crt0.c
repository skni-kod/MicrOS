#include "crt0.h"

void _start(/*char *path, char *parameters*/)
{
    // TODO: Split parameters to the separate elements in array
    //char *argv[3];
    //argv[0] = path;
    //argv[1] = parameters;
    //argv[2] = 0;

    exit(main(0, 0));
}