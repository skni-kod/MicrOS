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
		printf("[%d]", i);
		switch (((Object*)array->Data[i])->Type) {
		case String:
			printf("(String)->%s\n", ((char*)((Object*)array->Data[i])->Data));
			break;
		case Int:
			printf("(Int)->%d\n", *((int*)((Object*)array->Data[i])->Data));
			break;
		case Double:
			printf("(Double)->%lf\n", *((double*)((Object*)array->Data[i])->Data));
			break;
		default:
			printf("NaN\n");
			break;
		};
	}

}
