#ifndef PROCESS_USER_INFO_H
#define PROCESS_USER_INFO_H

typedef struct process_user_info
{
    uint32_t id;
    char name[32];

    float cpu_usage;
    uint32_t memory_usage;
} process_user_info;

#endif