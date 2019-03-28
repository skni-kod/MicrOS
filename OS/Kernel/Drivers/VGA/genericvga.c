#include "genericvga.h"
#include "../DAL/VideoCard/videocard.h"

VideoMode currentVideoMode;
uint8_t textMode;

void driverInit()
{
    DriverInitStruct s;
    s.setVideoMode = &setVideoMode;
    s.isTextMode = &isTextMode;
    s.getCurrentVideoMode = &getCurrentVideoMode;
    initWithDriver(s);
}

int16_t setVideoMode(uint16_t mode){
    switch(mode)
    {
        default: return -1;
    }
}

uint8_t isTextMode()
{
    return textMode;
}

VideoMode getCurrentVideoMode()
{
    return currentVideoMode;
}