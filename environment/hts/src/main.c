#include <stdio.h>
#include <micros.h>

int main(void)
{
    while(1)
    {
        printf("Alloc 1 MiB (USER)\n");
        malloc(1024*1024);
        printf("Done...");
        micros_keyboard_scan_ascii_pair kb;
        micros_keyboard_wait_for_key_press(&kb);
    }
}