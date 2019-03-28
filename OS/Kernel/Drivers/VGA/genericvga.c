#include "genericvga.h"
#include "../DAL/VideoCard/videocard.h"
#include "modes/mode_13h.h"

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
        case 0x13:
            if(setMode13H() != 0x13) return -1;
            
            return 0x13;
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