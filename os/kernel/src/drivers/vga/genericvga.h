#ifndef GENERICVGA_H
#define GENERICVGA_H

#include <stdint.h>
#include "drivers/dal/videocard/videocard.h"

void driverInit();

int16_t setVideoModeVGA(uint16_t mode);
uint8_t isTextModeVGA();
VideoMode *getCurrentVideoModeVGA();

#endif