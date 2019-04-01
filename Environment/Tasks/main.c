#include <micros.h>
#include <stdlib.h>

void draw_process_tree(micros_process_user_info *processes, int root_id, int level, int count);
void draw_bar(uint32_t filled_entries, uint32_t total_entries);
void draw_cpu_usage_bar(uint32_t cpu_usage, uint32_t bar_length);
void draw_memory_usage_bar(micros_physical_memory_stats *memory_stats, uint32_t bar_length);
void print_process_info(micros_process_user_info *process_info);
void print_memory_stats(micros_physical_memory_stats *memory_stats);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("TASKS");

    while (1)
    {
        micros_console_clear();

        micros_console_print_string("TASKS v1.1 @ MicrOS\n");
        micros_console_print_string("\n");

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

        micros_console_print_char('\n');

        draw_process_tree(processes, 0, 0, processes_count);

        micros_console_print_string("\n");
        print_memory_stats(&memory_stats);

        free(processes);
        micros_process_current_process_sleep(1500);
    }
    return 0;
}

void draw_process_tree(micros_process_user_info *processes, int root_id, int level, int count)
{
    micros_process_user_info *root_process = NULL;
    for (uint32_t i = 0; i < count; i++)
    {
        micros_process_user_info *process = &processes[i];
        if(process->id == root_id)
        {
            root_process = &processes[i];
        }
    }
    
    if(level != 0)
    {
        for(int i=0; i<level; i++)
        {
            micros_console_print_char(' ');
        }
                
        micros_console_print_string("> ");
    }
    
    print_process_info(root_process);
    micros_console_print_string("\n");
    
    for (uint32_t i = 0; i < count; i++)
    {
        micros_process_user_info *process = &processes[i];
        if(process->id != 0 && process->parent_id == root_id)
        {
            draw_process_tree(processes, process->id, level + 2, count);
        }
    }
}

void draw_bar(uint32_t filled_entries, uint32_t total_entries)
{
    for (uint32_t i = 0; i < filled_entries; i++)
    {
        micros_console_print_char('#');
    }

    for (uint32_t i = 0; i < total_entries - filled_entries; i++)
    {
        micros_console_print_char(' ');
    }
}

void draw_cpu_usage_bar(uint32_t cpu_usage, uint32_t bar_length)
{
    char buffer[32];

    uint32_t filled_entries = (cpu_usage / 10) * bar_length / 100;
    filled_entries = bar_length < filled_entries ? bar_length : filled_entries;

    micros_console_print_string("CPU: [");
    draw_bar(filled_entries, bar_length);
    micros_console_print_string("] ");

    itoa(cpu_usage / 10, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" %\n");
}

void draw_memory_usage_bar(micros_physical_memory_stats *memory_stats, uint32_t bar_length)
{
    char buffer[32];

    uint32_t memory_usage = memory_stats->allocated_entries + memory_stats->reserved_entries;
    uint32_t total_memory = memory_usage + memory_stats->free_entries;

    micros_console_print_string("MEM: [");
    draw_bar(memory_usage * bar_length / total_memory, bar_length);
    micros_console_print_string("] ");

    itoa(memory_usage * 100 / total_memory, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" %\n");
}

void print_process_info(micros_process_user_info *process_info)
{
    char buffer[32];

    micros_console_print_string(process_info->name);
    micros_console_print_string(" - ");

    micros_console_print_string("ID: ");
    itoa(process_info->id, buffer, 10);
    micros_console_print_string(buffer);
    
    if(process_info->id != process_info->parent_id)
    {
        micros_console_print_string(", Parent: ");
        itoa(process_info->parent_id, buffer, 10);
        micros_console_print_string(buffer);
    }

    micros_console_print_string(", Status: ");
    itoa(process_info->status, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string("");

    micros_console_print_string(", CPU: ");
    itoa(process_info->cpu_usage / 10, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" %");

    micros_console_print_string(", MEMORY: ");
    itoa(process_info->memory_usage, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" MB");
}

void print_memory_stats(micros_physical_memory_stats *memory_stats)
{
    char buffer[32];

    micros_console_print_string("Total memory usage: ");
    itoa((memory_stats->reserved_entries + memory_stats->allocated_entries) * 4, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" MB (");
    itoa(memory_stats->allocated_entries * 4, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" MB allocated, ");
    itoa(memory_stats->reserved_entries * 4, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" MB reserved, ");
    itoa(memory_stats->free_entries * 4, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" MB free).");
}