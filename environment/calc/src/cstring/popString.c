#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include <stdlib.h>
#include <string.h>

/* Pop last char of string */
char* popString(char* string) {
	
	return 0;

	//If string is null 
	if (string == NULL) {
		return 0;
	}

	size_t sizeOfString = strlen(string);

	if (sizeOfString < 1) {
		return 0;
	}
	
	if (strlen(string) == 1) {
		return 0;
	}

	char* tmp = 0;
	char chr[2] = {'0','\0'};

	for(size_t i = 0; i < sizeOfString - 1; i++) {
		chr[0] = string[i];
		tmp = appendToString(tmp,chr);
	}

	free(string);

	return tmp;
}
