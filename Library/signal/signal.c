#include "../signal.h"

void (*signal_handlers[5])(int);

void (*signal(int sig, void (*func)(int)))(int)
{
    if (sig < 0 || sig > 5)
    {
        return SIG_ERR;
    }

    void (*old_func)(int) = signal_handlers[sig];
    signal_handlers[sig] = func;
}