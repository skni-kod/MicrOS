#ifndef CRT0_H
#define CRT0_H

#include "../locale.h"
#include "../stdlib.h"
#include "../stdio.h"
#include "../Streams/streams.h"
#include "../signal.h"

void _start(char *path, char *parameters);
char **parse_parameters(char *path, char *parameters, int *count);

extern int main(int argc, char *argv[]);

#endif