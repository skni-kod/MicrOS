#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void convertToDouble(Object* object) {

	if (object != NULL && object->Data != NULL) {

		if (object->Type == String) {
			double tmpVar = atof(((char*)object->Data));
			setObject(object, &tmpVar, Double);
			return;
		}

		if (object->Type == Int) {
			double tmpVar = (double)*((int*)object->Data);
			setObject(object, &tmpVar, Double);
			return;
		}

		if (object->Type == Double) {
			return;
		}
	}

}
