#include "../fenv.h"

int fegetenv(fenv_t *envp)
{
    if(envp == NULL)
    {
        return 1;
    }
    *envp = _FPU_read_environment();
    return 0;
}