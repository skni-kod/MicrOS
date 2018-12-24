#ifndef TIME_H
#define TIME_H

#define CLOCKS_PER_SEC 1000

#include <stdint.h>
#include "micros.h"

typedef uint32_t clock_t;

clock_t clock();

#endif