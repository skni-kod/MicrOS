#include "../signal.h"

void (*signal_handlers[5])(int);

void (*signal(int sig, signal_func func))(int)
{
    if (sig < 0 || sig > 5)
    {
        return (signal_func)SIG_ERR;
    }

    signal_func old_func = signal_handlers[sig];
    signal_handlers[sig] = func;

    return old_func;
}