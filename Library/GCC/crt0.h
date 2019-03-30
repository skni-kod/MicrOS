#ifndef CRT0_H
#define CRT0_H

#include "../locale.h"
#include "../stdlib.h"
#include "../stdio.h"
#include "../stdio/streams/streams.h"
#include "../signal.h"
#include "../micros.h"

void _start(char *path, char *parameters);
char **parse_parameters(char *path, char *parameters, int *count);
void signal_handler(micros_signal_params *old_state);

extern int main(int argc, char *argv[]);

#endif