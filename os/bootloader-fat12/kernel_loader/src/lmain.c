#include "drivers/serial/serial.h"

int loader_main(void){

    //In order to debug this code you can use this binary file:
    // /os/bootloader-fat12/kernel_loader/bin/microldr.elf
    // if compilation fails with error like:
    /* === Linking output file bin/microldr.elf ===
    /opt/cross/bin/../lib/gcc/i386-elf/10.2.0/../../../../i386-elf/bin/ld: cannot find -lc
    collect2: error: ld returned 1 exit status
    */
    // You must compile C library by yourself before linking,
    // or move library building section before building second stage is build.sh file

    serial_init(COM1_PORT, 1200, 8, 1, PARITY_NONE);
    serial_send_string(COM1_PORT,"DUPA\n\r");

}