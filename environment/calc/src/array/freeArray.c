#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>

/* Delete array */
void freeArray(void** array, size_t* sizeOfArray, Type type)
{
	if ((array != NULL) && (sizeOfArray != NULL)) {
		switch (type) {
		case tString:
		{
			char** Array = (char**)array;
			//Delete all values
			for (size_t i = 0; i < *sizeOfArray; i++) {
				if (Array[i] != NULL) {
					free(Array[i]);
				}
			}
		}
		case tInt:
		{
			int** Array = (int**)array;
			//Delete all values
			for (size_t i = 0; i < *sizeOfArray; i++) {
				if (Array[i] != NULL) {
					free(Array[i]);
				}
			}
		}
		case tDouble:
		{
			double** Array = (double**)array;
			//Delete all values
			for (size_t i = 0; i < *sizeOfArray; i++) {
				if (Array[i] != NULL) {
					free(Array[i]);
				}
			}
		}
		break;
		};
	}

	*sizeOfArray = 0;
}
