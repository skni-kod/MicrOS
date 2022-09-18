#include <micros/micros_socket.h>
#include <stdio.h>

#define BUF_LEN 2000

int main(int argc, char *argv[])
{
    uint32_t sock = micros_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 5555);
    char buffer[BUF_LEN] = {0};
    while (1)
    {
        micros_recv(sock, buffer, BUF_LEN);
        printf("%s", buffer);
    }
    return 0;
}