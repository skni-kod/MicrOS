#include "startupMisc.h"
#include "../Drivers/PCSpeaker/pcspeaker.h"
#include "../../../Library/stdlib.h"

uint8_t bpm = 126;

uint32_t quaterNote()
{
    return 60000/bpm;
}

uint32_t halfNote()
{
    return quaterNote() * 2;
}

uint32_t wholeNote()
{
    return quaterNote() * 4;
}

uint32_t eighthNote()
{
    return quaterNote()/2;
}

uint32_t sixteenthNote()
{
    return quaterNote()/4;
}

void setBPM(uint8_t new_bpm){
    bpm = new_bpm;
}

void playNote(uint32_t freq, uint32_t time){
    sound(freq);
    sleep(time);
    nosound();
}

void whatIsLove()
{
    setBPM(126);
    playNote(DS5_NOTE, eighthNote());
    playNote(FS5_NOTE, eighthNote());
    playNote(GS5_NOTE, eighthNote() + halfNote());
    playNote(DS5_NOTE, eighthNote());
    playNote(E5_NOTE, eighthNote());
    playNote(DS5_NOTE, eighthNote());
    playNote(FS5_NOTE, eighthNote() + eighthNote());
    playNote(DS5_NOTE, halfNote() + eighthNote());
    playNote(DS5_NOTE, eighthNote());
    playNote(FS5_NOTE, eighthNote() + eighthNote());
    playNote(DS5_NOTE, halfNote() + eighthNote());
    playNote(DS5_NOTE, eighthNote());
    playNote(CS5_NOTE, eighthNote() + halfNote() + quaterNote());
}