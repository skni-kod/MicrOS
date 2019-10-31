#include "../signal.h"

int raise_with_param(int sig, int param)
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

    signal_handlers[sig] = __signal_get_defualt_handler(sig);

    func(param);
    return 0;
}