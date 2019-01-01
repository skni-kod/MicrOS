#include <micros.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SHUKA");
    while (1)
    {
        uint32_t now = micros_timer_get_system_clock();
        while (now + 100 >= micros_timer_get_system_clock())
            ;

        micros_process_current_process_sleep(90);
    }
    return 0;
}