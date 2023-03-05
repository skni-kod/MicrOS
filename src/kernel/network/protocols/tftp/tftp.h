#ifndef TFTP_H
#define TFTP_H

typedef enum tftp_opcode
{
    RRQ = 1,
    WRQ = 2,
    DATA = 3,
    ACK = 4,
    ERROR = 5,
    OPT_ACK = 6
} tftp_opcode_t;

#endif