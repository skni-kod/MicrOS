#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdio.h>

/* Print array to std out*/
void printArray(void** array, size_t* sizeOfArray, Type type) {

	//If array is empty
	if (*sizeOfArray < 1 || array == NULL) {
		printf("Array is empty\n");
		return;
	}

	//Print each value in format: [index]->{value}
	switch (type) {
	case tString:
		{
			char** Array = (char**)array;
			for (size_t i = 0; i < *sizeOfArray; i++) {
				printf("[%zu]->{%s}\n", i, Array[i]);
			}
		}
		break;
	case tInt:
		{
			int** Array = (int**)array;
			for (size_t i = 0; i < *sizeOfArray; i++) {
				printf("[%zu]->{%i}\n", i, *Array[i]);
			}
		}
		break;
	case tDouble:
		{
			double** Array = (double**)array;
			for (size_t i = 0; i < *sizeOfArray; i++) {
				printf("[%zu]->{%lf}\n", i, *Array[i]);
			}
		}
		break;
	};

}
