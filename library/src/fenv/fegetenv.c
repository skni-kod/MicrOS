#include "../fenv.h"

int fegetenv(fenv_t *envp)
{
    if(envp == NULL)
    {
        return 1;
    }
    *envp = __FPU_read_environment();
    return 0;
}