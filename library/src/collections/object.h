#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdbool.h>
#include "../stdio.h"
#include "../stdlib.h"
#include "../stdlib.h"

typedef enum
{
  String,
  Int,
  Double,
  Pointer,
} Type;

typedef struct
{
  void *Data;
  size_t Size;
  Type Type;
} Object;

void __objectReportError(const int errorCode);
Object *__objectInit(size_t typeSize, Type type);
Object *__objectInitDouble(double data);
Object *__objectInitDoublePointer(double *data);
Object *__objectInitInt(int data);
Object *__objectInitIntPointer(int *data);
Object *__objectInitChar(char data);
Object *__objectInitCharPointer(char *data);
Object *__objectInitPointer(void *data);
bool __objectCheckData(void *data);

/*
    Initialize new data object, with specified value
*/
#define ObjectInit(VALUE) _Generic((VALUE),                     \
                                   double                       \
                                   : __objectInitDouble,        \
                                     double *                   \
                                   : __objectInitDoublePointer, \
                                     int                        \
                                   : __objectInitInt,           \
                                     int *                      \
                                   : __objectInitIntPointer,    \
                                     char                       \
                                   : __objectInitChar,          \
                                     char *                     \
                                   : __objectInitCharPointer,   \
                                     void *                     \
                                   : __objectInitPointer)(VALUE)

#endif //OBJECT_H