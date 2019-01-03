#include "startup_music.h"

uint8_t bpm = 126;

uint32_t startup_music_quater_note()
{
    return 60000 / bpm;
}

uint32_t startup_music_half_note()
{
    return startup_music_quater_note() * 2;
}

uint32_t startup_music_whole_note()
{
    return startup_music_quater_note() * 4;
}

uint32_t startup_music_eighth_note()
{
    return startup_music_quater_note() / 2;
}

uint32_t startup_music_sixteenth_note()
{
    return startup_music_quater_note() / 4;
}

void startup_music_set_bmp(uint8_t new_bpm)
{
    bpm = new_bpm;
}

void startup_music_play_note(uint32_t freq, uint32_t time)
{
    pc_speaker_enable_sound(freq);
    sleep(time);
    pc_speaker_disable_sound();
}

void startup_music_play()
{
    startup_music_set_bmp(126);
    startup_music_play_note(DS5_NOTE, startup_music_eighth_note());
    startup_music_play_note(FS5_NOTE, startup_music_eighth_note());
    startup_music_play_note(GS5_NOTE, startup_music_eighth_note() + startup_music_half_note());
    startup_music_play_note(DS5_NOTE, startup_music_eighth_note());
    startup_music_play_note(E5_NOTE, startup_music_eighth_note());
    startup_music_play_note(DS5_NOTE, startup_music_eighth_note());
    startup_music_play_note(FS5_NOTE, startup_music_eighth_note() + startup_music_eighth_note());
    startup_music_play_note(DS5_NOTE, startup_music_half_note() + startup_music_eighth_note());
    startup_music_play_note(DS5_NOTE, startup_music_eighth_note());
    startup_music_play_note(FS5_NOTE, startup_music_eighth_note() + startup_music_eighth_note());
    startup_music_play_note(DS5_NOTE, startup_music_half_note() + startup_music_eighth_note());
    startup_music_play_note(DS5_NOTE, startup_music_eighth_note());
    startup_music_play_note(CS5_NOTE, startup_music_eighth_note() + startup_music_half_note() + startup_music_quater_note());
}