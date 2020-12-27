#include <micros.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SHUTDOWN");
    micros_power_shutdown();
    
    micros_process_current_process_sleep(1000);
    printf("If SHUTDOWN command doesn't work, then it may mean that OS is running on real hardware, " \
           "where this function is not supported.\n");
           
    return 0;
}