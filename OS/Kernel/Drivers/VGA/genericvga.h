#ifndef GENERICVGA_H
#define GENERICVGA_H

#include <stdint.h>

void driverInit();

int16_t setVideoMode(uint16_t mode);
uint8_t isTextMode();
VideoMode getCurrentVideoMode();

#endif