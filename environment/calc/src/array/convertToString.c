#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void convertToString(Object* object) {

	if (object != NULL && object->Data != NULL) {
		
		if (object->Type == String) {
			return;
		}

		if (object->Type == Int) {
			int value = *((int*)object->Data);
			//Sign of value
			bool valSign = value >= 0 ? true : false;
			Object* tmp = initObject("", String);
			if (value == 0 || value == NULL) {
				appendToString(tmp, "0");
				setObject(object, tmp->Data, String);
				return;
			}
			//Absolute value
			if (!valSign) {
				value *= -1;
			}
			while (true) {
				//Temporary char
				char tmpChar[2] = { '0' + (value % 10), '\0' };
				appendToString(tmp, tmpChar);
				value = value / 10;
				if (value == 0) { break; }
			}

			//Leading sign
			if (!valSign) {
				appendToString(tmp, "-");
			}

			reverseString(tmp);

			setObject(object, tmp->Data, String);
			return;
		}

		if (object->Type == Double) {
			double value = *((double*)object->Data);

			bool valueSign = value >= 0 ? true : false;

			Object* tmp = initObject("", String);

			if (!valueSign) {
				value = -1 * value;
			}

			int tmpVar = (int)value;
			Object* intPart = initObject(&tmpVar, Int);
			tmpVar = (int)((value - (double)tmpVar) * 1000000000);
			Object* fracPart = initObject(&tmpVar, Int);
			

			if (!valueSign) {
				appendToString(tmp, "-");
			}

			convertToString(intPart);
			appendToString(tmp, ((char*)intPart->Data));

			appendToString(tmp, ".");

			convertToString(fracPart);
			appendToString(tmp, ((char*)fracPart->Data));

			setObject(object, tmp->Data, String);
			trimString(object,true,'0');
			return;
		}
	}

}
