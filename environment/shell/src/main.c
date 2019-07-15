#include <micros.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SHUKA");
    while (1)
    {
        micros_console_clear();
        // uint32_t now = micros_timer_get_system_clock();
        // while (now + 100 >= micros_timer_get_system_clock())
        //     ;

        char a = getchar();
        printf("asd %c\n", a);
        scanf("%c", &a);

        micros_keyboard_scan_ascii_pair pair;
        micros_keyboard_wait_for_key_press(&pair);

        char b[16];
        micros_console_print_string("ASCII: ");
        micros_console_print_string(itoa(pair.ascii, b, 10));
        micros_console_print_string(", code: ");
        micros_console_print_string(itoa(pair.scancode, b, 10));
        micros_console_print_string("\n");

        //micros_process_current_process_sleep(200);
    }
    return 0;
}