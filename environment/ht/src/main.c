#include <stdio.h>
#include <micros.h>

int main(void)
{
    while(1)
    {
        printf("Alloc 1 MiB\n");
        micros_heap_kernel_alloc(1024*1024, 0);
        printf("Done...");
        micros_keyboard_scan_ascii_pair kb;
        micros_keyboard_wait_for_key_press(&kb);
    }
}