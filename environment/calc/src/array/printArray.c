#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Print array */
void printArray(Array* array) {

	if (array == NULL) {
		return;
	}
	
	for (unsigned int i = 0; i < array->Size; i++) {
		if (((Object*)array->Data[i])->Data != NULL) {
			printf("[%d]", i);
			switch (((Object*)array->Data[i])->Type) {
			case String:
				printf("(String){%d}->%s\n", ((size_t)((Object*)array->Data[i])->Length), ((char*)((Object*)array->Data[i])->Data));
				break;
			case Int:
				printf("(Int)->%d\n", *((int*)((Object*)array->Data[i])->Data));
				break;
			case Double:
				printf("(Double)->%lf\n", *((double*)((Object*)array->Data[i])->Data));
				break;
			case Pointer:
				printf("(Pointer)->%p\n", ((void*)((Object*)array->Data[i])->Data));
				break;
			default:
				printf("NaN\n");
				break;
			};
		}
	}
	if (array->Size < 1) {
		printf("Array is empty\n");
	}

}
