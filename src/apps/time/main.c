#include <micros.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_rtc_time time;
    micros_rtc_read_time(&time);
    
    printf("%02d-%02d-%d %02d:%02d:%02d\n",
        (int)time.day,
        (int)time.month,
        (int)time.year,
        (int)time.hour,
        (int)time.minute,
        (int)time.second);
        
    return 0;
}