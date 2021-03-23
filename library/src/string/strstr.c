#include "../string.h"

/*char *strstr(const char *str1, const char *str2)
{
    const char *ret = str2;
    int i;
    while(*str1 != '\0')
    {
        //*******************************************************
        for(i=0 ; *str1 == *str2 && *str2 != '\0' ; str1++ , str2++ , i++) // magic for dont touch
        //*******************************************************
        if(*str2 == '\0')
            return (char *)(str1 - i);
        else
            str2 = ret;
        str1++;
    }
    return 0;
}   */

int compare(const char *X, const char *Y)
{
	while (*X && *Y)
	{
		if (*X != *Y)
			return 0;

		X++;
		Y++;
	}

	return (*Y == '\0');
}

// Function to implement strstr() function
char* strstr(const char* str1, const char* str2)
{
	while (*str1 != '\0')
	{
		if ((*str1 == *str2) && compare(str1, str2))
			return str1;
		str1++;
	}

	return NULL;
}