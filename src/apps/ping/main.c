#include <micros/sys/micros_socket.h>
#include <micros/sys/micros_netif.h>
#include <inet/ipv4.h>
#include <inet/inet.h>
#include <micros/sys/micros_process.h>
#include <micros/sys/micros_keyboard.h>
#include <micros/sys/micros_rtc.h>
#include <micros/sys/micros_console.h>

#define BUF_LEN 1500

static void __print_netif_info(net_interface_t *iface)
{
    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           iface->mac.octet_a,
           iface->mac.octet_b,
           iface->mac.octet_c,
           iface->mac.octet_d,
           iface->mac.octet_e,
           iface->mac.octet_f);
    printf("IPv4: %d.%d.%d.%d\n",
           iface->ipv4_address.oct_a,
           iface->ipv4_address.oct_b,
           iface->ipv4_address.oct_c,
           iface->ipv4_address.oct_d);
    printf("Netmask: %d.%d.%d.%d\n",
           iface->ipv4_netmask.oct_a,
           iface->ipv4_netmask.oct_b,
           iface->ipv4_netmask.oct_c,
           iface->ipv4_netmask.oct_d);
    printf("GW: %d.%d.%d.%d\n",
           iface->ipv4_gateway.oct_a,
           iface->ipv4_gateway.oct_b,
           iface->ipv4_gateway.oct_c,
           iface->ipv4_gateway.oct_d);
    printf("DNS: %d.%d.%d.%d\n",
           iface->ipv4_dns.oct_a,
           iface->ipv4_dns.oct_b,
           iface->ipv4_dns.oct_c,
           iface->ipv4_dns.oct_d);
    printf("Dropped frames: %d\n", iface->frames_dropped);
    printf("RX frames: %d     bytes: %d\n", iface->frames_received, iface->bytes_received);
    printf("TX frames: %d     bytes: %d\n", iface->frames_sent, iface->bytes_sent);
}

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
        myaddr.sin_addr.value = INADDR_ANY;

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
        return 0;
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
        myaddr.sin_addr.value = INADDR_ANY;

        bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

        listen(sock, 1);

        int conn;
        while ((conn = accept(sock, &server_addr, &server_addr_len)) < 1)
            micros_process_current_process_sleep(1);

        printf("Connection from: %d.%d.%d.%d:%d",
               server_addr.sin_addr.oct_a,
               server_addr.sin_addr.oct_b,
               server_addr.sin_addr.oct_c,
               server_addr.sin_addr.oct_d,
               server_addr.sin_port);

        int n = 0;
        uint32_t bytes_received;
        char send_buffer[BUF_LEN] = {0};

        printf(">");

        // infinite loop for chat
        for (;;)
        {
            memset(buffer, 0, BUF_LEN);

            // read the message from client and copy it in buffer
            bytes_received = recv(conn, buffer, sizeof(buffer), 0);
            if (bytes_received)
            {
                printf(":%s", buffer);
                send(conn, buffer, bytes_received, 0);
            }
            // if ((send_buffer[n++] = getchar()) != '\n')
            //     ;
            // else
            // {
            //     send_buffer[n++] = 0;
            //     send(conn, send_buffer, strlen(send_buffer), 0);
            //     printf(">%s", send_buffer);
            //     memset(send_buffer, 0, BUF_LEN);
            // }

            // if (micros_keyboard_is_key_pressed())
            // {
            //     micros_keyboard_scan_ascii_pair pressed_key;
            //     micros_keyboard_get_pressed_key(&pressed_key);

            //     switch (pressed_key.scancode)
            //     {
            //     case key_esc:
            //         return;
            //     }
            // }
        }
        return 0;
    }

    if (!strcmp("-lookup", argv[2]))
    {
        printf("Looking up for IP of: %s\n", argv[3]);

        ipv4_addr_t addr = dns_lookup(argv[3]);

        printf("%s has:  %d.%d.%d.%d\n",
               argv[3],
               addr.oct_a,
               addr.oct_b,
               addr.oct_c,
               addr.oct_d);
        return 0;
    }

    if (!strcmp("-client", argv[2]))
    {
        uint32_t sock = 0;
        enum socket_domain proto;
        if (!strcmp("-tcp", argv[3]))
        {
            sock = socket(AF_INET, SOCK_STREAM, 0);
            proto = SOCK_STREAM;
        }
        if (!strcmp("-udp", argv[3]))
        {
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            proto = SOCK_DGRAM;
        }

        if (!sock)
            return -1;

        uint16_t port = atoi(argv[5]);
        ipv4_addr_t srv;

        srv = inet_addr(argv[4]);

        struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_zero = 0,
            .sin_port = htons(port),
            .sin_addr.value = srv.value};

        socklen_t server_addr_len = sizeof(struct sockaddr_in);

        if (proto == SOCK_STREAM && connect(sock, &server_addr, server_addr_len))
        {
            printf("Unable to connect");
            return -1;
        }

        printf("Connected to: %d.%d.%d.%d:%d\n",
               server_addr.sin_addr.oct_a,
               server_addr.sin_addr.oct_b,
               server_addr.sin_addr.oct_c,
               server_addr.sin_addr.oct_d,
               port);

        char buffer[512];
        char snd_buffer[512];
        uint32_t n = 0;
        micros_rtc_time time;
        char chr;
        micros_console_position pos;
        micros_console_set_cursor_visibility(true);

        while (1)
        {

            if (micros_keyboard_is_key_pressed())
            {
                micros_keyboard_scan_ascii_pair pressed_key;
                micros_keyboard_get_pressed_key(&pressed_key);

                switch (pressed_key.scancode)
                {
                case key_esc:
                    return;
                case key_backspace:
                    snd_buffer[n--] = 0;
                    micros_console_get_cursor_position(&pos);
                    pos.x--;
                    micros_console_set_cursor_position(&pos);
                    micros_console_print_char(' ');
                    micros_console_set_cursor_position(&pos);
                    break;
                case key_enter:
                    snd_buffer[n++] = '\n';
                    snd_buffer[n++] = 0;
                    switch (proto)
                    {
                    case SOCK_DGRAM:
                        sendto(sock, snd_buffer, strlen(snd_buffer), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
                        break;
                    case SOCK_STREAM:
                        send(sock, snd_buffer, strlen(snd_buffer), 0);
                        break;
                    }

                    micros_rtc_read_time(&time);
                    printf("\n%02d:%02d:%02d [MicrOS]> %s",
                           time.hour,
                           time.minute,
                           time.second,
                           snd_buffer);
                    memset(snd_buffer, 0, 512);
                    n = 0;
                    break;
                default:
                    snd_buffer[n++] = chr = pressed_key.ascii;
                    if (chr == '\n')
                    {
                        snd_buffer[n++] = '\r';
                        snd_buffer[n++] = '\n';
                    }
                    printf("%c", chr);
                }
            }

            uint32_t bytes;
            switch (proto)
            {
            case SOCK_DGRAM:
                bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr,
                                 &server_addr_len);
                break;

            case SOCK_STREAM:
                bytes = recv(sock, buffer, sizeof(buffer), 0);
                break;
            }

            if (bytes)
            {
                micros_rtc_read_time(&time);
                printf("%02d:%02d:%02d [%d.%d.%d.%d]> %s",
                       time.hour,
                       time.minute,
                       time.second,
                       server_addr.sin_addr.oct_a,
                       server_addr.sin_addr.oct_b,
                       server_addr.sin_addr.oct_c,
                       server_addr.sin_addr.oct_d,
                       buffer);
                memset(buffer, 0, 512);
            }
        }
        return 0;
    }

    if (!strcmp("-show", argv[2]))
    {
        net_interface_t iface;
        micros_netif_get(atoi(argv[3]), &iface);
        __print_netif_info(&iface);
        return 0;
    }

    if (!strcmp("-ip", argv[2]))
    {
        net_interface_t iface;
        uint32_t iface_id = atoi(argv[3]);

        micros_netif_get(iface_id, &iface);
        iface.ipv4_address = inet_addr(argv[4]);
        micros_netif_set(iface_id, &iface);
        micros_netif_get(iface_id, &iface);
        __print_netif_info(&iface);
        return 0;
    }

    if (!strcmp("-netmask", argv[2]))
    {
        net_interface_t iface;
        uint32_t iface_id = atoi(argv[3]);

        micros_netif_get(iface_id, &iface);
        iface.ipv4_netmask = inet_addr(argv[4]);
        micros_netif_set(iface_id, &iface);
        micros_netif_get(iface_id, &iface);
        __print_netif_info(&iface);
        return 0;
    }

    if (!strcmp("-gateway", argv[2]))
    {
        net_interface_t iface;
        uint32_t iface_id = atoi(argv[3]);

        micros_netif_get(iface_id, &iface);
        iface.ipv4_gateway = inet_addr(argv[4]);
        micros_netif_set(iface_id, &iface);
        micros_netif_get(iface_id, &iface);
        __print_netif_info(&iface);
        return 0;
    }

    if (!strcmp("-dns", argv[2]))
    {
        net_interface_t iface;
        uint32_t iface_id = atoi(argv[3]);

        micros_netif_get(iface_id, &iface);
        iface.ipv4_dns = inet_addr(argv[4]);
        micros_netif_set(iface_id, &iface);
        micros_netif_get(iface_id, &iface);
        __print_netif_info(&iface);
        return 0;
    }

    return 0;
}