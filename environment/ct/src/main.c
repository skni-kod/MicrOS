#include <time.h>
#include <stdlib.h>
#include <micros.h>

int main(void)
{
    clock_t begin_time;
    while(1)
    {
        //begin_time = clock();
        micros_process_current_process_sleep(1000);
        printf("MS: %d \n", clock() - begin_time);   
    }
    return 0;
}