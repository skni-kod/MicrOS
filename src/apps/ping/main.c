#include <micros/sys/micros_socket.h>

#define BUF_LEN 2000

int main(int argc, char *argv[])
{
    char buffer[BUF_LEN] = {0};
    uint32_t sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.207");

    socklen_t server_addr_len = sizeof(struct sockaddr_in);

    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(5555);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

    while (1)
    {
        ssize_t bytes_received = recvfrom(sock,
                                          buffer,
                                          sizeof(buffer),
                                          0,
                                          (struct sockaddr *)&server_addr,
                                          &server_addr_len);
        if (bytes_received){
            printf("%.*s\n", bytes_received, buffer);
            sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_addr_len);
        }
    }

    return 0;
}