#include <micros.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SHUKA");
    while (1)
        ;
    return 0;
}