#include "tftp.h"
#include <inet/inet.h>

int sock;

struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_zero = 0,
    .sin_port = htons(69),
    .sin_addr = {.oct_a = 192, .oct_b = 168, .oct_c = 99, .oct_d = 1}};

socklen_t server_addr_len = sizeof(struct sockaddr_in);

int tftp_connect()
{
    sock = k_socket(AF_INET, SOCK_DGRAM, 0);
    if (!sock)
        return -1;
}

size_t __strlen(const char *str)
{
    size_t length = 0;

    while (str[length] != 0)
    {
        length++;
    }

    return length;
}

int __strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
        str1++, str2++;
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

int __atoi(const char *string)
{
    size_t len = strlen(string);
    int value = 0;
    int position = 1;

    char sign = 1;
    char lengthOffset = 0;
    if (*string == '-')
    {
        sign = -1;
        lengthOffset = 1;
    }

    for (int8_t i = len - 1; i >= lengthOffset; --i)
    {
        value += (string[i] - '0') * position;
        position *= 10;
    }

    return value * sign;
}

uint32_t tftp_get_file_size(const char *file)
{
    uint8_t test[4096 * 10];
    uint32_t header_size = 0;
    uint32_t rv  = 0 ;

    // write opcode:
    *((uint16_t *)&test) = htons(RRQ);
    header_size += 2;

    header_size += kernel_sprintf(test + header_size, "%s", file);
    *(test + header_size++) = 0;
    header_size += kernel_sprintf(test + header_size, "octet");
    *(test + header_size++) = 0;
    header_size += kernel_sprintf(test + header_size, "blksize");
    *(test + header_size++) = 0;
    header_size += kernel_sprintf(test + header_size, "%d",512);
    *(test + header_size++) = 0;
    header_size += kernel_sprintf(test + header_size, "tsize");
    *(test + header_size++) = 0;
    header_size += kernel_sprintf(test + header_size, "%d",0);
    *(test + header_size++) = 0;


    k_sendto(sock, test, header_size, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

    uint32_t bytes;

    while (!(bytes = recvfrom(sock, test, sizeof(4096), 0, (struct sockaddr *)&server_addr,
                              &server_addr_len)))
        ;
    for (uint32_t i = 0; i < bytes; i++)
    {
        if (0 == __strcmp(test + i, "tsize"))
        {
            rv = __atoi(test + i + 5);
            break;
        }
    }

    header_size = 0;
    *((uint16_t *)&test) = htons(ACK);
    header_size += 2;
    *(test + header_size++) = 0;
    *(test + header_size++) = 0;
    k_sendto(sock, test, header_size, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

    header_size = 0;
    *((uint16_t *)&test) = htons(ERROR);
    header_size += 2;
    *(test + header_size++) = 0;
    *(test + header_size++) = 0;
    header_size += kernel_sprintf(test + header_size, "Dupa!");
    *(test + header_size++) = 0;
    k_sendto(sock, test, header_size, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    
    return rv;
}