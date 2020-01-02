#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** splitString(char* string, const char delimiter, size_t* sizeOfArray) {
	//Output array
	char** array = 0;
	
	//Temporary string
	char* tmp = 0;

	if (string == NULL) {
		return 0;
	}

	size_t sizeOfString = strlen(string);

	if(sizeOfString < 1){
		return 0;
	}

	//Current character	
	char value[2] = { 0,'\0' };

	for (size_t i = 0; i <= sizeOfString; i++) {
		value[0] = string[i];
		if (value[0] == delimiter || value[0] == '\0') {
			array = appendToArray(array,tmp,sizeOfArray,tString);
			tmp = emptyString(tmp);
		}else{
			tmp = appendToString(tmp, value);
		}
	}

	return array;
}
