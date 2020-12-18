#include "../stdio.h"
#include "../errno.h"

char **errors = {"No error", "Math argument out of domain of func", "Math result not representable", "Illegal byte sequence"};

void perror(const char *str)
{
    if(str != NULL)
    {
        fprintf(stderr, "%s: %s\n", str, errors[errno]);   
    }
    else
    {
        fprintf(stderr, "%s\n", errors[errno]);   
    }
    
}