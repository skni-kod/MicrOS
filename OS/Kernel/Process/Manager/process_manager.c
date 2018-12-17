#include "process_manager.h"

extern void enter_user_space(uint32_t address);
vector processes;

void process_manager_init()
{
    vector_init(&processes);
}

void process_manager_start_process(char *path)
{
    process_header *process = (process_header *)heap_kernel_alloc(sizeof(process_header), 0);
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

    __asm__("mov %0, %%eax\n"
            "mov %%eax, %%esp"
            :
            : "g"(process->stack)
            : "eax");

    enter_user_space(app_header->entry_position);
}