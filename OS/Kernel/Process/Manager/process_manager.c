#include "process_manager.h"

vector processes;
uint32_t next_process_id = 0;

extern void enter_user_space(uint32_t address);

void process_manager_init()
{
    vector_init(&processes);
}

uint32_t process_manager_create_process(char *path)
{
    process_header *process = (process_header *)heap_kernel_alloc(sizeof(process_header), 0);
    process->id = next_process_id++;
    process->page_directory = heap_kernel_alloc(1024 * 4, 1024 * 4);

    paging_table_entry *page_directory = paging_get_page_directory();
    memcpy(process->page_directory, (void *)page_directory, 1024 * 4);

    paging_set_page_directory((uint32_t)process->page_directory);

    uint16_t sectors, size;
    uint8_t *content = fat12_read_file(path, &sectors, &size);

    elf_header *app_header = elf_get_header(content);
    uint32_t initial_page = elf_loader_load(content);

    process->base_heap_page_index = initial_page + 1;
    heap_set_user_heap((void *)(process->base_heap_page_index * 1024 * 1024 * 4));
    heap_init_user_heap();

    process->stack = heap_user_alloc(1024 * 1024, 0) + 1024 * 1024;

    vector_add(&processes, process);
    heap_kernel_dealloc(content);

    return process->id;
}

process_header *process_manager_get_process(uint32_t process_id)
{
    for (int i = 0; i < processes.count; i++)
    {
        if (((process_header *)processes.data[i])->id == process_id)
        {
            return (process_header *)processes.data[i];
        }
    }
}