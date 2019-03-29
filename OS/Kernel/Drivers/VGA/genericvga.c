#include "genericvga.h"
#include "../DAL/VideoCard/videocard.h"
#include "modes/mode_13h/mode_13h.h"

VideoMode currentVideoMode;
uint8_t textMode;

void driverInit()
{
    DriverInitStruct s;
    s.setVideoMode = &setVideoModeVGA;
    s.isTextMode = &isTextModeVGA;
    s.getCurrentVideoMode = &getCurrentVideoModeVGA;
    initWithDriver(s);
}

int16_t setVideoModeVGA(uint16_t mode){
    switch(mode)
    {
        case 0x03:
            turnOffBuffer();
        case 0x04:
            turnOffBuffer();
        case 0x05:
            turnOffBuffer();
        case 0x06:
            turnOffBuffer();
        case 0x0D:
            turnOffBuffer();
        case 0x0E:
            turnOffBuffer();
        case 0x0F:
            turnOffBuffer();
        case 0x10:
            turnOffBuffer();
        case 0x11:
            turnOffBuffer();
        case 0x12:
            turnOffBuffer();
        case 0x13:
            turnOffBuffer();
            if(setMode13H() != 0x13) return -1;
            return 0x13;
        case 0x69:
            turnOffBuffer();
        default: return -1;
    }
}

uint8_t isTextModeVGA()
{
    return textMode;
}

VideoMode getCurrentVideoModeVGA()
{
    return currentVideoMode;
}