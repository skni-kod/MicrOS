#include "../vector.h"

/* Initialize empty vector */
Vector *VectorInit(void)
{
    Vector *vec = NULL;
    vec = malloc(sizeof(vec));

    if (vec != NULL)
    {
        vec->Size = 0;
        vec->Data = NULL;
        return vec;
    }
    else
        ReportError(-1);
}
