#ifndef CRT0_H
#define CRT0_H

#include "../stdlib.h"

void _start(char *path, char *parameters);
char **parse_parameters(char *path, char *parameters, int *count);

extern int main(int argc, char *argv[]);

#endif