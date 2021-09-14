#include <stdio.h>
#include "string/tests.h"

#define ASSERTION_ERROR 1
#define ASSERTION_SUCCESS 0

int main(int argc, char *argv[])
{
    if (test_memcpy()==ASSERTION_SUCCESS) printf("MEMCPY test successful!\n");
    else printf("MEMCPY test failed!");

    return 0;
}

