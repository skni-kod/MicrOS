#ifndef MICROS_GDI
#define MICROS_GDI

#include "micros_interrupts.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct _video_mode_descriptor
{
    uint16_t mode_number;
    uint32_t width;
    uint32_t height;
    uint64_t colors;
    uint8_t text;
    uint8_t grayscale;
    uint8_t planar;
} video_mode_descriptor;

/*!
    Find video mode that best suits specified params in vmd
    \param vmd Pointer to video_mode_descriptor structure containing mode parameters. After succesfful find it stores parameters of choosen mode.
    \return 0 on success, otherwise -1.
*/
int8_t micros_gdi_find_video_mode(video_mode_descriptor* vmd);

/*!
    Sets specified video mode
    \param mode Number of mode we want to switch to.
    \pre Obtain mode number using micros_gdi_find_video_mode
*/
void micros_gdi_set_video_mode(uint16_t mode);

/*! Checks if the currently selected VGA mode is text mode
    \return True if it's text mode, otherwise false.
*/
bool micros_gdi_is_text_mode();

/*!
    Returns number of the currently selected VGA mode.
    \return VGA mode number.
*/
uint16_t micros_gdi_get_current_video_mode();

#endif