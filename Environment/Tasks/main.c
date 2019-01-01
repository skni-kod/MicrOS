#include <micros.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint32_t last_update_time = micros_timer_get_system_clock();
    micros_process_set_current_process_name("TASKS");

    while (1)
    {
        char buffer[32];

        micros_console_clear();
        micros_console_print_string("TASKS v1.0 @ MicrOS\n");
        micros_console_print_string("_______________________________________________\n");
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

        micros_physical_memory_stats memory_stats;
        micros_memory_get_physical_memory_stats(&memory_stats);

        micros_console_print_string("\n");
        micros_console_print_string("Total CPU usage: ");
        itoa(total_cpu_usage / 10, buffer, 10);
        micros_console_print_string(buffer);
        micros_console_print_string(" %");

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
        micros_process_current_process_sleep(1000);
    }
    return 0;
}