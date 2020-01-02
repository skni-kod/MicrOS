#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "cstring.h"
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv)
{
	
	char* string = 0;
	size_t size = 0;
	char** array = 0;

	string = appendToString(string,"Hello;World;123123");

	array = splitString(string,';',&size);
	
	//*(array + (*sizeOfArray - 1))
	printArray(array,&size,tString);

	return 0;
}
