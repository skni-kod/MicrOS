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
			for (size_t i = 0; i < *sizeOfArray; i++) {
				printf("[%zu]->{%s}\n", i, *(array + i));
			}
		}
		break;
	case tInt:
		{
			for (size_t i = 0; i < *sizeOfArray; i++) {
				printf("[%zu]->{%i}\n", i, *(array + i));
			}
		}
		break;
	case tDouble:
		{
			for (size_t i = 0; i < *sizeOfArray; i++) {
				printf("[%zu]->{%lf}\n", i, *(array + i));
			}
		}
		break;
	};

}
