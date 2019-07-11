#include <micros.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SNAKE");
    micros_console_clear();
    
    while(1);
    return 0;
}