#include "../signal.h"

void (*signal_handlers[6])(int);

int raise(int sig)
{
    if (sig < 0 || sig > 5)
    {
        return SIG_ERR;
    }

    signal_func func = signal_handlers[sig];
    if (func == SIG_IGN)
    {
        return SIG_ERR;
    }

    func(0);
    return 0;
}