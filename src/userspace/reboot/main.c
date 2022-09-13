#include <micros.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("REBOOT");
    micros_power_reboot();
    
    return 0;
}