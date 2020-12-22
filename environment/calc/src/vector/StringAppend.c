#include "../vector.h"

/* Append text to string */
void StringAppend(Object *object, const char *string)
{

	if (object == NULL || object->Type != String)
	{
		ReportError(-2);
		return;
	}

	size_t stringLen = ObjectDataSize(string, String);
	if (stringLen > 0)
	{
		object->Size += sizeof(char) * stringLen;
		/* Concatenate two strings in new variable */
		char *tmp = NULL;
		tmp = malloc(object->Size);
		if (tmp != NULL)
		{
			strcpy(tmp, object->Data);
			strcat(tmp, string);

			free(object->Data);
			object->Data = tmp;
		}
		else
			ReportError(-1);
	}
	else
		ReportError(-3);
}
