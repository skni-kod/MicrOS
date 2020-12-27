#include <stdio.h>
#include <collections.h>
int main(int argc, char *argv[])
{
	Object *tmp = ObjectInit(-0.123456789112);
	ObjectPrint(tmp);
	ObjectToString(tmp);
	ObjectPrint(tmp);
	ObjectSet(tmp, 343742742);
	ObjectPrint(tmp);
	ObjectToString(tmp);
	ObjectPrint(tmp);
	ObjectSet(tmp, 0.123456f);
	ObjectPrint(tmp);
	ObjectToString(tmp);
	ObjectPrint(tmp);
	return 0;
}