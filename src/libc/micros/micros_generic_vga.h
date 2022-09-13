#ifndef MICROS_GENERIC_VGA_H
#define MICROS_GENERIC_VGA_H

#include <stdbool.h>
#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Sets VGA video mode
/*!
    Sets VGA video mode to the specified in the parameter.
    \param mode VGA mode.
*/
void micros_generic_vga_set_video_mode(uint16_t mode);

//! Checks if the currently selected VGA mode is text mode
/*!
    Checks if the currently selected VGA mode is text mode.
    \return True if it's text mode, otherwise false.
*/
bool micros_generic_vga_is_text_mode();

//! Returns number of the currently selected VGA mode
/*!
    Returns number of the currently selected VGA mode.
    \return VGA mode number.
*/
uint16_t micros_generic_vga_get_current_video_mode();

#ifdef __cplusplus
}
#endif

#endif