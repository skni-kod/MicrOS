#include "genericvga.h"
#include "drivers/dal/videocard/videocard.h"
#include "modes/mode_03h/mode_03h.h"
#include "modes/mode_04h/mode_04h.h"
#include "modes/mode_05h/mode_05h.h"
#include "modes/mode_06h/mode_06h.h"
#include "modes/mode_0Dh/mode_0Dh.h"
#include "modes/mode_0Eh/mode_0Eh.h"
#include "modes/mode_0Fh/mode_0Fh.h"
#include "modes/mode_10h/mode_10h.h"
#include "modes/mode_11h/mode_11h.h"
#include "modes/mode_12h/mode_12h.h"
#include "modes/mode_13h/mode_13h.h"
#include "modes/mode_y/mode_y.h"

VideoMode currentVideoMode;
uint8_t textMode;

void driverInit()
{
    DriverInitStruct s;
    s.setVideoMode = &setVideoModeVGA;
    s.isTextMode = &isTextModeVGA;
    s.getCurrentVideoMode = &getCurrentVideoModeVGA;
    s.turnOnBuffer = &turnOnBuffer03H;
    s.turnOffBuffer = &turnOffBuffer03H;
    s.isBufferOn = &isBufferOn03H;
    s.swapBuffers = &swapBuffers03H;
    s.drawPixel = &drawPixel03H;
    s.drawLine = &drawLine03H;
    s.drawCircle = &drawCircle03H;
    s.drawRectangle = &drawRectangle03H;
    s.clearScreen = &clearScreen03H;
    initWithDriver(s);

    currentVideoMode.colors = 16;
    currentVideoMode.height = 25;
    currentVideoMode.width = 80;
    currentVideoMode.id = 0x03;
    currentVideoMode.monochrome = 0;
    currentVideoMode.planar = 0;

    textMode = 1;
}

int16_t setVideoModeVGA(uint16_t mode){
    switch(mode)
    {
        case 0x03:
            turnOffBuffer();
            if(setMode03H() != 0x03) return -1;
            currentVideoMode.colors = 16;
            currentVideoMode.height = 25;
            currentVideoMode.width = 80;
            currentVideoMode.id = 0x03;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 0;
            textMode = 1;
            return 0x03;
        case 0x04:
            turnOffBuffer();
            if(setMode04H() != 0x04) return -1;
            currentVideoMode.colors = 4;
            currentVideoMode.height = 200;
            currentVideoMode.width = 320;
            currentVideoMode.id = 0x04;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 0;
            textMode = 0;
            return 0x04;
        case 0x05:
            turnOffBuffer();
            if(setMode05H() != 0x05) return -1;
            currentVideoMode.colors = 4;
            currentVideoMode.height = 200;
            currentVideoMode.width = 320;
            currentVideoMode.id = 0x05;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 0;
            textMode = 0;
            return 0x05;
        case 0x06:
            turnOffBuffer();
            if(setMode06H() != 0x06) return -1;
            currentVideoMode.colors = 2;
            currentVideoMode.height = 200;
            currentVideoMode.width = 640;
            currentVideoMode.id = 0x06;
            currentVideoMode.monochrome = 1;
            currentVideoMode.planar = 0;
            textMode = 0;
            return 0x06;
        case 0x0D:
            turnOffBuffer();
            if(setMode0DH() != 0x0D) return -1;
            currentVideoMode.colors = 16;
            currentVideoMode.height = 200;
            currentVideoMode.width = 320;
            currentVideoMode.id = 0x0D;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x0D;
        case 0x0E:
            turnOffBuffer();
            if(setMode0EH() != 0x0E) return -1;
            currentVideoMode.colors = 16;
            currentVideoMode.height = 200;
            currentVideoMode.width = 640;
            currentVideoMode.id = 0x0E;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x0E;
        case 0x0F:
            turnOffBuffer();
            if(setMode0FH() != 0x0F) return -1;
            currentVideoMode.colors = 4;
            currentVideoMode.height = 350;
            currentVideoMode.width = 640;
            currentVideoMode.id = 0x0F;
            currentVideoMode.monochrome = 1;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x0F;
        case 0x10:
            turnOffBuffer();
            if(setMode10H() != 0x10) return -1;
            currentVideoMode.colors = 16;
            currentVideoMode.height = 350;
            currentVideoMode.width = 640;
            currentVideoMode.id = 0x10;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x10;
        case 0x11:
            turnOffBuffer();
            if(setMode11H() != 0x11) return -1;
            currentVideoMode.colors = 2;
            currentVideoMode.height = 480;
            currentVideoMode.width = 640;
            currentVideoMode.id = 0x11;
            currentVideoMode.monochrome = 1;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x11;
        case 0x12:
            turnOffBuffer();
            if(setMode12H() != 0x12) return -1;
            currentVideoMode.colors = 16;
            currentVideoMode.height = 480;
            currentVideoMode.width = 640;
            currentVideoMode.id = 0x12;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x12;
        case 0x13:
            turnOffBuffer();
            if(setMode13H() != 0x13) return -1;
            currentVideoMode.colors = 256;
            currentVideoMode.height = 200;
            currentVideoMode.width = 320;
            currentVideoMode.id = 0x13;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 0;
            textMode = 0;
            return 0x13;
        case 0x69:
            turnOffBuffer();
            if(setModeY() != 0x69) return -1;
            currentVideoMode.colors = 256;
            currentVideoMode.height = 200;
            currentVideoMode.width = 320;
            currentVideoMode.id = 0x69;
            currentVideoMode.monochrome = 0;
            currentVideoMode.planar = 1;
            textMode = 0;
            return 0x69;
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