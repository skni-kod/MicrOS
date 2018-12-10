#ifndef STARTUP_MISC_H
#define STARTUP_MISC_H

#include <stdint.h>

#define C4_NOTE 262
#define CS4_NOTE 277
#define D4_NOTE 294
#define DS4_NOTE 311
#define E4_NOTE 330
#define F4_NOTE 439
#define FS4_NOTE 370
#define G4_NOTE	392
#define GS4_NOTE 415
#define A4_NOTE	440
#define AS4_NOTE 466
#define B4_NOTE	494
#define C5_NOTE	523
#define CS5_NOTE 554
#define D5_NOTE	587
#define DS5_NOTE 622
#define E5_NOTE	659
#define F5_NOTE	698
#define FS5_NOTE 740
#define G5_NOTE 784
#define GS5_NOTE 831
#define A5_NOTE 880



void setBPM(uint8_t new_bpm);
void playNote(uint32_t freq, uint32_t time);
void whatIsLove();


#endif