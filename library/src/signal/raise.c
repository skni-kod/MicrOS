#include "../signal.h"

int raise(int sig)
{
    return raise_with_param(sig, 0);
}