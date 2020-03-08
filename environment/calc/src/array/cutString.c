#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void cutString(Object* object, const unsigned int index, const int count) {

	if (object != NULL && object->Type == String && object->Data != NULL && count != 0){
		if (abs(count) < (object->Length - 1)) {
			unsigned int rangeBegin = 0, rangeEnd = 0;
			if (count > 0) {
				rangeBegin = (index - count + 1) > 0 ? (index - abs(count) + 1) : 0;
				rangeEnd = index > (object->Length - 2) ? (object->Length - 2) : index;
			}
			if (count < 0) {
				rangeBegin = index > (object->Length - 2) ? (object->Length - 2) : index;
				rangeEnd = (index + abs(count) - 1) > (object->Length - 2) ? (object->Length - 2) : (index + abs(count) - 1);
			}

			Object* tmp = initObject("", String);
			char tmpChar[2] = "";
			for (unsigned int i = 0; i < (object->Length - 1); i++) {
				if (!(i >= rangeBegin && i <= rangeEnd)) {
					tmpChar[0] = ((char*)object->Data)[i];
					appendToString(tmp, tmpChar);
				}
			}
			setObject(object, tmp->Data, String);
			return;
		}
		if (abs(count) == (object->Length - 1)) {
			setObject(object, "", String);
			return;
		}
	}

}
