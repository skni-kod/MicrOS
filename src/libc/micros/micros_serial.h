#ifndef MICROS_SERIAL_H
#define MICROS_SERIAL_H

#define MICROS_COM1_PORT 0x3F8
#define MICROS_COM2_PORT 0x2F8
#define MICROS_COM3_PORT 0x3E8
#define MICROS_COM4_PORT 0x2E8

#define MICROS_PARITY_NONE 0
#define MICROS_PARITY_ODD 1
#define MICROS_PARITY_EVEN 3
#define MICROS_PARITY_MARK 5
#define MICROS_PARITY_SPACE 7

#include <stdint.h>
#include <stdbool.h>
#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Inits serial port
/*!
    Inits serial port with the specified parameters
    \param port Serial port ID (MICROS_COMx_PORT).
    \param baud_rate Speed of the serial port in bauds.
    \param data_bits Data length (set 8 for default).
    \param stop_bits Stop bits for the single message (set 1 for default).
    \param parity Parity of the message (set MICROS_PARITY_NONE for default).
*/
void micros_serial_init(unsigned int port, unsigned int baud_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity);

//! Checks if the specified serial port is busy
/*!
    Checks if the specified serial port is busy.
    \param port Serial port ID (MICROS_COMx_PORT).
    \return True if the specified serial port is busy, otherwise false.
*/
bool micros_serial_is_busy(unsigned int port);

//! Checks if the specified serial port queue is empty
/*!
    Checks if the specified serial port queue is empty.
    \param port Serial port ID (MICROS_COMx_PORT).
    \return True if the specified serial port queue is empty, otherwise false.
*/
bool micros_serial_is_queue_empty(unsigned int port);

//! Sends a char on the specified serial port
/*!
    Sends a char on the specified serial port.
    \param port Serial port ID (MICROS_COMx_PORT).
    \param c Char to send.
*/
void micros_serial_send(unsigned int port, char c);

//! Sends a string on the specified serial port
/*!
    Sends a string on the specified serial port.
    \param port Serial port ID (MICROS_COMx_PORT).
    \param string String to send.
*/
void micros_serial_send_string(unsigned int port, char *str);


//! Reads a char on the specified serial port
/*!
    Reads a char on the specified serial port.
    \param port Serial port ID (MICROS_COMx_PORT).
    \return True if the specified serial port queue is empty, otherwise false.
*/
char micros_serial_receive(unsigned int port);

#ifdef __cplusplus
}
#endif

#endif