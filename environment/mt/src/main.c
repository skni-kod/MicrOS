#include <stdio.h>
#include <micros.h>
#include <micros/micros_console.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    while(1)
    {
        if(micros_keyboard_is_key_pressed())
        {
            micros_keyboard_scan_ascii_pair kb;
            micros_keyboard_get_pressed_key(&kb);
            unsigned char* ptr = (unsigned char*)malloc(5*1024*1024);
            *(uint32_t*)((uint8_t*)ptr + 4*1024*1024 + 256) = 0xB00BF00C;

            if(*(uint32_t*)((uint8_t*)ptr + 4*1024*1024 + 256) != 0xB00BF00C)
            {
                printf("HEAP ERROR!\n");
            }
            else
            {
                printf("HEAP OK!\n");
            }         
        }
    }
    return 0;
}