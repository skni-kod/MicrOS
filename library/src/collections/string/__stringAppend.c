#include "../object.h"

int __stringAppendChar(Object *object, char c)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return 0;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return 0;
    }

    size_t size = sizeof(char) + object->Size;
    object->Data = realloc(object->Data, size);
    if (object->Data != NULL)
    {
        *(char *)(object->Data + ((size / sizeof(char)) - 1)) = '\0';
        *(char *)(object->Data + ((size / sizeof(char)) - 2)) = c;
        object->Size = size;
        return 1;
    }
    else
        __objectReportError(-1);
    return 0;
}

int __stringAppendCharPointer(Object *object, char *str)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return 0;

    if (!__objectCheckPointer((void *)str))
        return 0;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return 0;
    }

    size_t size = sizeof(char) * strlen(str) + object->Size;
    object->Data = realloc(object->Data, size);
    if (object->Data != NULL)
    {
        strcat(object->Data, str);
        object->Size = size;
        return strlen(str);
    }
    else
        __objectReportError(-1);
    return 0;
}