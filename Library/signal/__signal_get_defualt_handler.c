#include "../signal.h"

signal_func __signal_get_defualt_handler(int sig)
{
    switch (sig)
    {
    case SIGABRT:
        return default_sigabrt_handler;
    case SIGFPE:
        return default_sigfpe_handler;
    case SIGILL:
        return default_sigill_handler;
    case SIGINT:
        return default_sigint_handler;
    case SIGSEGV:
        return default_sigsegv_handler;
    case SIGTERM:
        return default_sigterm_handler;
    }

    return (signal_func)SIG_ERR;
}