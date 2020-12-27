#include "../object.h"

bool __objectSetFloat(Object *object, float data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetFloatPointer(Object *object, float *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetDouble(Object *object, double data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetDoublePointer(Object *object, double *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetInt(Object *object, int data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetIntPointer(Object *object, int *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetUInt(Object *object, unsigned int data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetUIntPointer(Object *object, unsigned int *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetLong(Object *object, long data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetLongPointer(Object *object, long *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetULong(Object *object, unsigned long data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetULongPointer(Object *object, unsigned long *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetChar(Object *object, char data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetCharPointer(Object *object, char *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}

bool __objectSetPointer(Object *object, void *data)
{
    if (!__objectCheckPointer(object))
        return false;
    if (ObjectDelete(object))
        object = ObjectInit(data);
    else
        return false;
    if (__objectCheckPointer(object))
        return true;
}