#include "dns.h"

static uint16_t dns_id = 1;

ipv4_addr_t dns_lookup(const char *domain)
{
    ipv4_addr_t addr;

    memset(&addr, 0, 4);

    dns_header_t dns_lookup_header = {
        .id = htons(dns_id),
        .qdcount = htons(0x0001),
        .flags = htons(0x0100), // recursion requested
    };

    // Transforms a domain name into a DNS query name, e.g.:
    // foo.example.com -> 3foo7example3com
    uint32_t domain_len = strlen(domain);
    uint32_t data_len = (1            // first count
                         + domain_len // dots will be replaced with counts
                         + 1          // END
                         + 2          // Type
                         + 2          // Class
                         ) *
                        sizeof(uint8_t);

    uint8_t *data = heap_kernel_alloc(data_len, 0);

    char *_domain = heap_kernel_alloc((domain_len + 2) * sizeof(char), 0);
    memset(_domain, 0, (domain_len + 2) * sizeof(char));
    strcat(_domain, domain);
    strcat(_domain, ".");

    uint32_t j = 0, pos = 0;
    for (uint32_t i = 0; i < domain_len + 1; i++)
    {
        if (_domain[i] == '.')
        {
            // add the count first
            data[j++] = i - pos;

            // copy part
            while (pos < i)
            {
                data[j++] = _domain[pos++];
            }

            pos++;
        }
    }
    // END
    data[j++] = 0;
    // Type
    data[j++] = (uint8_t)(DNS_TYPE_A << 8);
    data[j++] = (uint8_t)(DNS_TYPE_A);
    // Class
    data[j++] = (uint8_t)(DNS_CLASS_IN << 8);
    data[j++] = (uint8_t)(DNS_CLASS_IN);

    heap_kernel_dealloc(_domain);

    uint16_t packet_len = sizeof(dns_header_t) + data_len;
    uint8_t *packet = heap_kernel_alloc(packet_len, 0);
    memcpy(packet, &dns_lookup_header, sizeof(dns_header_t));
    memcpy(packet + sizeof(dns_header_t), data, data_len);

    heap_kernel_dealloc(data);

    int sockfd = k_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sockfd < 0)
    {
        return (ipv4_addr_t){.value = 0};
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(DNS),
        .sin_addr = network_manager_get_nic()->interface.ipv4_dns};
    // ;
    socklen_t server_addr_len = sizeof(struct sockaddr_in);

    k_sendto(sockfd,
             packet,
             packet_len,
             0,
             (struct sockaddr *)&server_addr,
             server_addr_len);

    heap_kernel_dealloc(packet);
    dns_id++;
    uint8_t buf[128];

    uint64_t elapsed = 0;
    int recv = 0;
    while (elapsed++ < 0xFFFFF)
    {
        if (recv = k_recvfrom(sockfd, buf, 128, 0, &server_addr, sizeof(struct sockaddr_in)))
            break;
    }

    if (recv == 0)
        return (ipv4_addr_t){.value = 0};

    dns_header_t dns_header = {0};
    memcpy(&dns_header, buf, sizeof(dns_header_t));
    dns_header.id = ntohs(dns_header.id);
    dns_header.flags = ntohs(dns_header.flags);
    dns_header.qdcount = ntohs(dns_header.qdcount);
    dns_header.ancount = ntohs(dns_header.ancount);
    dns_header.nscount = ntohs(dns_header.nscount);
    dns_header.arcount = ntohs(dns_header.arcount);

    uint8_t *dns_data = buf + sizeof(dns_header_t);

    // Compute the query payload length so that we can skip it to read the
    // answers directly.
    uint16_t query_len = 0;
    for (uint16_t query = 0; query < dns_header.qdcount; query++)
    {
        while (dns_data[query_len++] != 0x00)
        {
            ;
        }
        query_len += 2; // type
        query_len += 2; // class
    }

    if (dns_header.ancount > 0)
    {
        // The following code only handles the first answer.
        dns_answer_header_t answer_header = {0};
        memcpy(&answer_header, dns_data + query_len, sizeof(dns_answer_header_t));
        answer_header.name = ntohs(answer_header.name);
        answer_header.type = ntohs(answer_header.type);
        answer_header.class = ntohs(answer_header.class);
        answer_header.ttl = ntohl(answer_header.ttl);
        answer_header.data_len = ntohs(answer_header.data_len);

        if (answer_header.class == DNS_CLASS_IN && answer_header.data_len == 4)
        {
            memcpy(&addr,
                   dns_data + query_len + sizeof(dns_answer_header_t),
                   answer_header.data_len);
        }
        else
        {
            return (ipv4_addr_t){.value = 0};
        }
    }
    else
    {
        return (ipv4_addr_t){.value = 0};
    }

    return addr;
}
