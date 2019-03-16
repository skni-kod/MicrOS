#include "../signal.h"

void (*signal_handlers[5])(int);

int raise(int sig)
{
    if (sig < 0 || sig > 5)
    {
        return SIG_ERR;
    }

    signal_handlers[sig](0);
    return 0;
}