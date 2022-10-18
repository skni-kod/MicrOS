#include <micros/sys/micros_socket.h>
#include <micros/sys/micros_netif.h>
#include <inet/ipv4.h>
#include <micros/sys/micros_process.h>
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

        uint32_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        struct sockaddr_in server_addr = {0};
        socklen_t server_addr_len = sizeof(struct sockaddr_in);

        struct sockaddr_in myaddr = {0};
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons(port);
        myaddr.sin_addr.address = INADDR_ANY;

        bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

        listen(sock, 4);

        int conn;
        while ((conn = accept(sock, &server_addr, &server_addr_len)) < 1)
            micros_process_current_process_sleep(1);

        int n = 0;
        uint32_t bytes_received;
        // infinite loop for chat
        for (;;)
        {
            memset(buffer, 0, BUF_LEN);

            // read the message from client and copy it in buffer
            bytes_received = recv(conn, buffer, sizeof(buffer), 0);
            if (bytes_received)
                printf("From client: %s", buffer);

            // n = 0;
            // // copy server message in the buffer
            // while ((buffer[n++] = getchar()) != '\n')
            //     ;
            // // if msg contains "Exit" then server exit and chat ended.
            // if (strncmp("exit", buffer, 4) == 0)
            // {
            //     printf("Server Exit...\n");
            //     return;
            // }
            // // and send that buffer to client
            // send(sock, buffer, strlen(buffer), 0);
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

    if (!strcmp("-dhcp", argv[2]))
    {
        printf("Sending DHCP request\n");

        uint32_t sock = socket(AF_INET, SOCK_DGRAM, 0);

        struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_zero = 0,
            .sin_port = htons(67),
            .sin_addr = INADDR_ANY};

        socklen_t server_addr_len = sizeof(struct sockaddr_in);

        struct sockaddr_in myaddr = {
            .sin_zero = 0,
            .sin_family = AF_INET,
            .sin_port = htons(68),
            .sin_addr.address = INADDR_ANY};

        int ret = bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

        uint32_t bytes_received;
        while (bytes_received = recvfrom(sock,
                                         buffer,
                                         sizeof(buffer),
                                         0,
                                         (struct sockaddr *)&server_addr,
                                         &server_addr_len))
        {
            // data came from server
            micros_process_current_process_sleep(1);
        }
        // if (bytes_received)
        // {
        //     printf("Received[%d]: %.*s \n", bytes_received, bytes_received, buffer);
        //     sendto(sock, buffer, bytes_received, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
        // }
    }

    if (!strcmp("-dropped", argv[2]))
        printf("Dropped frames: %d\n", nic_dropped());

    return 0;
}