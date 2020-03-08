#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Prints object */
void printObject(Object* object) {

	if ((object != NULL) && (object->Data != NULL)) {
		switch (object->Type)
		{
		case String:
			printf("(String){%d}->%s\n", ((size_t)object->Length), ((char*)object->Data));
			break;
		case Int:
			printf("(Int)->%d\n", *((int*)object->Data));
			break;
		case Double:
			printf("(Double)->%lf\n", *((double*)object->Data));
			break;
		case Pointer:
			printf("(Pointer)->%p\n", ((void*)object->Data));
			break;
		default:
			printf("NaN\n");
			break;
		}
	}
}
