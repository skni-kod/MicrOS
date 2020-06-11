#include "serial.h"

void serial_init(unsigned int port, unsigned int baud_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity)
{
    // Disable interrupts
    // ------------------
    io_out_byte(port + INTERRUPT_ENABLE_REGISTER, 0x00);
    
    // Set divisor
    // -----------
    uint16_t divisor = 115200 / baud_date;
    uint8_t divisor_low = divisor;
    uint8_t config_high = divisor >> 8;
    
    io_out_byte(port + LINE_CONTROL_REGISTER, 0x80);
    io_out_byte(port + DIVISOR_LOW_REGISTER, divisor_low);
    io_out_byte(port + DIVISOR_HIGH_REGISTER, config_high);
    
    // Set size, parity and stop bit
    // -----------------------------
    uint8_t config = (data_bits - 5) | ((stop_bits - 1) << 2) | (parity << 3);
    io_out_byte(port + LINE_CONTROL_REGISTER, config);
    
    // Enable and clearFIFO, 14-byte threshold
    // https://www.lammertbies.nl/comm/info/serial-uart#FCR
    // ----------------------------------------------------
    io_out_byte(port + INTERRUPT_FIFO_CONTROL_REGISTER, 0xC7);
    
    // Handshake with attached device
    io_out_byte(port + MODEM_CONTROL_REGISTER, 0x0B);
}