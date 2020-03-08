#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void convertToInt(Object* object) {

	if (object != NULL && object->Data != NULL) {

		if (object->Type == String) {
			int tmpVar = atoi(((char*)object->Data));
			printf("%s,%d\n",((char*)object->Data),tmpVar);
			setObject(object, &tmpVar, Int);
			return;
		}

		if (object->Type == Int) {
			return;
		}

		if (object->Type == Double) {
			int tmpVar = (int)*((double*)object->Data);
			setObject(object, &tmpVar, Int);
			return;
		}
	}

}
