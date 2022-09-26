#include <micros/sys/micros_socket.h>
#include <inet/ipv4.h>
#include <micros/sys/micros_keyboard.h>

#define BUF_LEN 2000

int main(int argc, char *argv[])
{
    char buffer[BUF_LEN] = {0};
    uint32_t sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in server_addr = {0};

    socklen_t server_addr_len = sizeof(struct sockaddr_in);

    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(40025);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

    while (1)
    {
        uint32_t bytes_received = recvfrom(sock,
                                           buffer,
                                           sizeof(buffer),
                                           0,
                                           (struct sockaddr *)&server_addr,
                                           &server_addr_len);

        printf("Received (%d): From: %d.%d.%d.%d:%d ",
               bytes_received,
               ((ipv4_addr_t *)&server_addr.sin_addr.s_addr)->oct_a,
               ((ipv4_addr_t *)&server_addr.sin_addr.s_addr)->oct_b,
               ((ipv4_addr_t *)&server_addr.sin_addr.s_addr)->oct_c,
               ((ipv4_addr_t *)&server_addr.sin_addr.s_addr)->oct_d,
               ntohs(server_addr.sin_port));
        if (bytes_received)
        {
            printf("%.*s  ", bytes_received, buffer);
            printf("Sent: %d\n", sendto(sock, buffer, bytes_received, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)));
        }
        else
        {
            printf("%s\n", "Process Alive");
        }

        if (micros_keyboard_is_key_pressed())
        {
            micros_keyboard_scan_ascii_pair pressed_key;
            micros_keyboard_get_pressed_key(&pressed_key);

            switch (pressed_key.scancode)
            {
            case key_esc:
                return;
            }
        }
    }

    return 0;
}