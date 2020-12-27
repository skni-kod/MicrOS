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
    void *tmp = NULL;
    tmp = malloc(size);
    if (tmp != NULL)
    {
        strcpy(tmp, object->Data);
        *(char *)(tmp + ((size / sizeof(char)) - 1)) = '\0';
        *(char *)(tmp + ((size / sizeof(char)) - 2)) = c;
        free(object->Data);
        object->Data = tmp;
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
    void *tmp = NULL;
    tmp = malloc(size);
    if (tmp != NULL)
    {
        strcpy(tmp, object->Data);
        strcat(tmp, str);
        free(object->Data);
        object->Data = tmp;
        object->Size = size;
        return strlen(str);
    }
    else
        __objectReportError(-1);
    return 0;
}