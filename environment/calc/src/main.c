#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv)
{
	Object* obj = initObject("HelloWorld!",String);
	Array array;
	initArray(&array);
	appendToArray(&array,obj);
	appendToArray(&array,obj);
	appendToArray(&array,obj);
	setData(obj,"Ocean ",String);
	appendToArray(&array,obj);
	appendToString(obj,"Drive");
	appendToArray(&array,obj);
	double x = 3.14;
	setData(obj,&x,Double);
	appendToArray(&array,obj);
	printArray(&array);
	return 0;
}
