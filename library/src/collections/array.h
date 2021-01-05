#ifndef ARRAY_H
#define ARRAY_H

#include "object.h"
#include "../stdlib.h"

/*

int func(int a,int b, int c){
  printf("%d %d %d \n",a,b,c);
  return a+b+c;
}

#define JUST3(a, b, c, ...) (a), (b), (c)
#define FUNC(...) func(JUST3(__VA_ARGS__, 0, 0))

  printf("%d\n",FUNC(1));
  printf("%d\n",FUNC(1,2));
  printf("%d\n",FUNC(1,2,3));

*/

typedef struct
{
    Object **Data;
    size_t Size;
    unsigned int Iterator;
} Array;
//Array append albo Object, albo wartosc ktora jest zmieniona na object
//! Initialize new array
/*!
    Get pointer to new object that stores passed value.
    \param VALUE Value stored in object. Can be passed both ways.
    \return Pointer to new array.
*/
Array *ArrayInit();
//append object,value, or elements of another array
//przy dopisywaniu elementu, argument opcjolanly usuwajacy to co przeslano
//wyjatek to zwykle wartosci ktore nie powinny byc jedynie kopiowane
void ArrayAppend(Array *array, Object *object, const bool copy);
void ArraySet(Array *array, Object *object, const unsigned int index);
void ArrayInsert(Array *array, Object *object, const unsigned int index);
void ArrayPrint(Array *array);
void ArrayDelete(Array *array);
void ArrayRemove(Array *array, const unsigned int index);
Object *ArrayPop(Array *array);
Array *StringSplit(Object *object, const char *delimiter);
Object *ArrayNext(Array *array);
Object *ArrayPrev(Array *array);
bool ArrayIsEnd(Array *array);
bool ArrayIsBegin(Array *array);
#endif //ARRAY_H