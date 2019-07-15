#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

void draw_process_tree(micros_process_user_info *processes, uint32_t root_id, uint32_t level, uint32_t count);
void print_total_process_count(int process_count);
void draw_bar(uint32_t filled_entries, uint32_t total_entries);
void draw_cpu_usage_bar(uint32_t cpu_usage, uint32_t bar_length);
void draw_memory_usage_bar(micros_physical_memory_stats *memory_stats, uint32_t bar_length);
void print_process_info(micros_process_user_info *process_info);
void print_memory_stats(micros_physical_memory_stats *memory_stats);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("TASKS");
    printf("TASKS");

    while (1)
    {
        micros_console_clear();

        printf("TASKS v1.1 @ MicrOS\n");
        printf("\n");

        uint32_t processes_count = micros_process_get_processes_count();
        micros_process_user_info *processes = malloc(sizeof(micros_process_user_info) * processes_count);

        micros_process_get_all_processes_info(processes);
        uint32_t total_cpu_usage = 0;
        uint32_t total_memory_usage = 0;

        for (uint32_t i = 0; i < processes_count; i++)
        {
            micros_process_user_info *process = &processes[i];
            total_cpu_usage += process->cpu_usage;
            total_memory_usage += process->memory_usage;
        }

        micros_physical_memory_stats memory_stats;
        micros_memory_get_physical_memory_stats(&memory_stats);

        draw_cpu_usage_bar(total_cpu_usage, 66);
        draw_memory_usage_bar(&memory_stats, 66);

        printf("\n");

        draw_process_tree(processes, 0, 0, processes_count);
        print_total_process_count(processes_count);

        printf("\n\n");
        print_memory_stats(&memory_stats);

        free(processes);
        micros_process_current_process_sleep(1500);
    }
    return 0;
}

void draw_process_tree(micros_process_user_info *processes, uint32_t root_id, uint32_t level, uint32_t count)
{
    micros_process_user_info *root_process = NULL;
    for (uint32_t i = 0; i < count; i++)
    {
        micros_process_user_info *process = &processes[i];
        if (process->id == root_id)
        {
            root_process = &processes[i];
        }
    }

    if (level != 0)
    {
        for (uint32_t i = 0; i < level; i++)
        {
            printf(" ");
        }

        printf("> ");
    }

    print_process_info(root_process);
    printf("\n");

    for (uint32_t i = 0; i < count; i++)
    {
        micros_process_user_info *process = &processes[i];
        if (process->id != 0 && process->parent_id == root_id)
        {
            draw_process_tree(processes, process->id, level + 2, count);
        }
    }
}

void print_total_process_count(int process_count)
{
    printf("Total: %d", process_count);
}

void draw_bar(uint32_t filled_entries, uint32_t total_entries)
{
    for (uint32_t i = 0; i < filled_entries; i++)
    {
        printf("#");
    }

    for (uint32_t i = 0; i < total_entries - filled_entries; i++)
    {
        printf(" ");
    }
}

void draw_cpu_usage_bar(uint32_t cpu_usage, uint32_t bar_length)
{
    uint32_t filled_entries = (cpu_usage / 10) * bar_length / 100;
    filled_entries = bar_length < filled_entries ? bar_length : filled_entries;

    printf("CPU: [");
    draw_bar(filled_entries, bar_length);
    printf("] ");

    printf("%d %%\n", cpu_usage / 10);
}

void draw_memory_usage_bar(micros_physical_memory_stats *memory_stats, uint32_t bar_length)
{
    uint32_t memory_usage = memory_stats->allocated_entries + memory_stats->reserved_entries;
    uint32_t total_memory = memory_usage + memory_stats->free_entries;

    printf("MEM: [");
    draw_bar(memory_usage * bar_length / total_memory, bar_length);
    printf("] ");

    printf("%d %%\n", memory_usage * 100 / total_memory);
}

void print_process_info(micros_process_user_info *process_info)
{
    printf("%s - ID: %d", process_info->name, process_info->id);

    if (process_info->id != process_info->parent_id)
    {
        printf(", Parent: %d", process_info->parent_id);
    }

    printf(", Status: %d, CPU: %d %%, MEMORY: %d MB", 
        process_info->status,
        (int)(process_info->cpu_usage / 10), 
        process_info->memory_usage);
}

void print_memory_stats(micros_physical_memory_stats *memory_stats)
{
    printf("Total memory usage: %d MB (%d MB allocated, %d MB reserved, %d MB free)",
        (memory_stats->reserved_entries + memory_stats->allocated_entries) * 4,
        memory_stats->allocated_entries * 4,
        memory_stats->reserved_entries * 4,
        memory_stats->free_entries * 4);
}