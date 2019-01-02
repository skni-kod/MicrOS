#include <micros.h>
#include <stdlib.h>

void draw_bar(int filled_entries, int total_entries);
void draw_cpu_usage_bar(int cpu_usage, int bar_length);
void draw_memory_usage_bar(micros_physical_memory_stats *memory_stats, int bar_length);

int main(int argc, char *argv[])
{
    uint32_t last_update_time = micros_timer_get_system_clock();
    micros_process_set_current_process_name("TASKS");

    while (1)
    {
        char buffer[32];
        micros_console_clear();

        micros_console_print_string("TASKS v1.0 @ MicrOS\n");
        micros_console_print_string("\n");

        uint32_t processes_count = micros_process_get_processes_count();
        micros_process_user_info *processes = malloc(sizeof(micros_process_user_info) * processes_count);

        micros_process_get_all_processes_info(processes);
        uint32_t total_cpu_usage = 0;
        uint32_t total_memory_usage = 0;

        for (int i = 0; i < processes_count; i++)
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

        for (int i = 0; i < processes_count; i++)
        {
            micros_process_user_info *process = &processes[i];
            micros_console_print_string(process->name);
            micros_console_print_string(" - ");

            micros_console_print_string("ID: ");
            itoa(process->id, buffer, 10);
            micros_console_print_string(buffer);

            micros_console_print_string(", Status: ");
            itoa(process->status, buffer, 10);
            micros_console_print_string(buffer);
            micros_console_print_string("");

            micros_console_print_string(", CPU: ");
            itoa(process->cpu_usage / 10, buffer, 10);
            micros_console_print_string(buffer);
            micros_console_print_string(" %");

            micros_console_print_string(", MEMORY: ");
            itoa(process->memory_usage, buffer, 10);
            micros_console_print_string(buffer);
            micros_console_print_string(" MB");

            micros_console_print_string("\n");
        }

        micros_console_print_string("\n");
        micros_console_print_string("Total memory usage: ");
        itoa((memory_stats.reserved_entries + memory_stats.allocated_entries) * 4, buffer, 10);
        micros_console_print_string(buffer);
        micros_console_print_string(" MB (");
        itoa(memory_stats.allocated_entries * 4, buffer, 10);
        micros_console_print_string(buffer);
        micros_console_print_string(" MB allocated, ");
        itoa(memory_stats.reserved_entries * 4, buffer, 10);
        micros_console_print_string(buffer);
        micros_console_print_string(" MB reserved, ");
        itoa(memory_stats.free_entries * 4, buffer, 10);
        micros_console_print_string(buffer);
        micros_console_print_string(" MB free).");

        free(processes);
        micros_process_current_process_sleep(1500);
    }
    return 0;
}

void draw_bar(int filled_entries, int total_entries)
{
    for (int i = 0; i < filled_entries; i++)
    {
        micros_console_print_char('#');
    }

    for (int i = 0; i < total_entries - filled_entries; i++)
    {
        micros_console_print_char(' ');
    }
}

void draw_cpu_usage_bar(int cpu_usage, int bar_length)
{
    char buffer[32];

    micros_console_print_string("CPU: [");
    draw_bar((cpu_usage / 10) * bar_length / 100, bar_length);
    micros_console_print_string("] ");

    itoa(cpu_usage / 10, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" %\n");
}

void draw_memory_usage_bar(micros_physical_memory_stats *memory_stats, int bar_length)
{
    char buffer[32];

    int memory_usage = memory_stats->allocated_entries + memory_stats->reserved_entries;
    int total_memory = memory_usage + memory_stats->free_entries;

    micros_console_print_string("MEM: [");
    draw_bar(memory_usage * bar_length / total_memory, bar_length);
    micros_console_print_string("] ");

    itoa(memory_usage * 100 / total_memory, buffer, 10);
    micros_console_print_string(buffer);
    micros_console_print_string(" %\n");
}