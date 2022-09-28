#include <micros/sys/micros_socket.h>
#include <micros/sys/micros_netif.h>
#include <inet/ipv4.h>
#include <micros/sys/micros_keyboard.h>

#define BUF_LEN 1500

int main(int argc, char *argv[])
{
    char buffer[BUF_LEN] = {0};

    if (!strcmp("-udp", argv[2]))
    {
        uint16_t port = atoi(argv[3]);
        printf("Listening on UDP port:%d\n", port);

        uint32_t sock = socket(AF_INET, SOCK_DGRAM, 0);

        struct sockaddr_in server_addr = {0};
        socklen_t server_addr_len = sizeof(struct sockaddr_in);

        struct sockaddr_in myaddr = {0};
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons(port);
        myaddr.sin_addr.address = INADDR_ANY;

        int ret = bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));
        while (!ret)
        {
            uint32_t bytes_received = recvfrom(sock,
                                               buffer,
                                               sizeof(buffer),
                                               0,
                                               (struct sockaddr *)&server_addr,
                                               &server_addr_len);
            if (bytes_received)
            {
                printf("Received[%d]: %.*s \n", bytes_received, bytes_received, buffer);
                sendto(sock, buffer, bytes_received, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
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
    }

    if (!strcmp("-tcp", argv[2]))
    {
        uint16_t port = atoi(argv[3]);
        printf("Listening on TCP port:%d\n", port);

        uint32_t sock = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server_addr = {0};
        socklen_t server_addr_len = sizeof(struct sockaddr_in);

        struct sockaddr_in myaddr = {0};
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons(port);
        myaddr.sin_addr.address = INADDR_ANY;

        int ret = bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

        ret = listen(sock, 4);

        while (!ret)
        {
            int conn = accept(sock, &server_addr, &server_addr_len);
            uint32_t bytes_received = recv(sock,
                                           buffer,
                                           sizeof(buffer),
                                           0);
            if (bytes_received)
            {
                printf("Received[%d]: %.*s \n", bytes_received, bytes_received, buffer);
                send(sock, buffer, bytes_received, 0);
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
    }

    if (!strcmp("-dropped", argv[2]))
        printf("Dropped frames: %d\n", nic_dropped());

    return 0;
}