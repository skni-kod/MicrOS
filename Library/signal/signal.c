#include "../signal.h"

void (*signal_handlers[6])(int);

void (*signal(int sig, signal_func func))(int)
{
    if ((uint32_t)func == SIG_DFL)
    {
        switch (sig)
        {
        case SIGABRT:
            func = default_sigabrt_handler;
            break;
        case SIGFPE:
            func = default_sigfpe_handler;
            break;
        case SIGILL:
            func = default_sigill_handler;
            break;
        case SIGINT:
            func = default_sigint_handler;
            break;
        case SIGSEGV:
            func = default_sigsegv_handler;
            break;
        case SIGTERM:
            func = default_sigterm_handler;
            break;
        }
    }

    if (sig < 0 || sig > 5)
    {
        return (signal_func)SIG_ERR;
    }

    signal_func old_func = signal_handlers[sig];
    signal_handlers[sig] = func;

    return old_func;
}