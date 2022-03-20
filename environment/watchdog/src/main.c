#include <micros.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    micros_rtc_time *time,*new_time,*tmp;
    time = malloc(sizeof(micros_rtc_time));
    new_time = malloc(sizeof(micros_rtc_time));

    while(1){
        micros_rtc_read_time(new_time);

        if(new_time->second > time->second)
            printf("Tic, toc!\r\n");

        tmp = time;
        time = new_time;
        new_time = tmp;
    }
}