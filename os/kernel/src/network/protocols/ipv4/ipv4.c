#include "ipv4.h"

void ipv4_process_packet(ipv4_packet_t *packet, uint8_t *device_mac)
{
    *(packet->version_ihl_ptr) = __uint8_flip(*(packet->version_ihl_ptr));

    //TODO:
    /*
    (TOS)
    FOR FUTURE USAGE OF DSCP and ECN, 
    IT IS NECESSARY TO FLIP THEIR VALUES
    */

    packet->length = __uint16_flip(packet->length);
    packet->id = __uint16_flip(packet->id);

    int x = 0;
}