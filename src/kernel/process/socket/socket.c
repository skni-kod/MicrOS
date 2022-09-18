#include "socket.h"

#define NB_SYSTEM_DESCRIPTORS 20
#define ENTRY_SIZE 32 * 128
descriptor_t descriptors[NB_SYSTEM_DESCRIPTORS] = {0};

uint16_t socket_base_port = 49152;

uint32_t create_socket_descriptor(uint32_t domain,
                                  uint32_t type,
                                  uint32_t protocol,
                                  uint32_t port)
{
    descriptor_t *descriptor;
    for (uint8_t fd = 3; fd < NB_SYSTEM_DESCRIPTORS; fd++)
    {
        descriptor = &descriptors[fd];
        if (!descriptor)
        {
            descriptor->used = true;
            descriptor->domain = domain;
            descriptor->type = type;
            descriptor->protocol = protocol;
            descriptor->port = port;
            descriptor->entry = heap_kernel_alloc(sizeof(sock_entry_t) + ENTRY_SIZE, 0);
            memset(descriptor->entry, 0, sizeof(sock_entry_t) + ENTRY_SIZE);
            descriptor->entry->len = ENTRY_SIZE;
            return fd;
        }
    }

    return -1;
}

uint32_t descriptor_udp_lookup(uint16_t port)
{
    for (uint8_t fd = 3; fd < NB_SYSTEM_DESCRIPTORS; fd++)
    {
        if (descriptors[fd].used && descriptors[fd].type == SOCK_DGRAM &&
            descriptors[fd].protocol == IPPROTO_UDP &&
            descriptors[fd].port == port)
        {
            return fd;
        }
    }

    return -1;
}

descriptor_t *get_descriptor(uint32_t id)
{
    if (id >= NB_SYSTEM_DESCRIPTORS)
    {
        return NULL;
    }

    return &descriptors[id];
}

uint32_t k_socket(uint32_t domain, uint32_t type, uint32_t protocol, uint32_t port)
{
    if (domain != AF_INET)
    {
        return 0;
    }

    if (type != SOCK_DGRAM)
    {
        return 0;
    }

    uint32_t sd = create_socket_descriptor(domain, type, protocol, port);

    return sd;
}

uint32_t k_recvfrom(uint32_t sock, void *buffer, size_t len)
{
    descriptor_t *desc = get_descriptor(sock);
    return entry_read(desc->entry, buffer, len);
}

uint32_t entry_read(sock_entry_t *entry, uint8_t *buffer, size_t len)
{
    uint16_t end = ENTRY_SIZE / 32;
    uint16_t ptr = 0;

    // read ptr follows write pointer in round buffer
    while (entry->read != entry->write)
    {
        // input buffer is full, do not copy data there!
        if (ptr >= len)
            return ptr;

        memcpy(buffer, entry->buffer[entry->read++ * 32], 32);

        if (entry->read == end)
            entry->read = 0;
        ptr += 32;
    }

    return ptr;
}

uint32_t entry_write(sock_entry_t *entry, uint8_t *buffer, size_t len)
{
    uint16_t end = ENTRY_SIZE / 32;
    uint16_t ptr = 0;

    // read ptr follows write pointer in round buffer
    while (entry->read != entry->write)
    {
        // input buffer is full, do not copy data there!
        if (ptr >= len)
            return ptr;

        memcpy(entry->buffer[entry->write++ * 32], buffer, 32);

        if (entry->write == end)
            entry->write = 0;
        ptr += 32;
    }

    return ptr;
}