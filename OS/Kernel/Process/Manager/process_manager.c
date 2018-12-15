#include "process_manager.h"

extern void enter_user_space(uint32_t address);
vector processes;

void process_manager_init()
{
    vector_init(&processes);
}

void process_manager_start_process(char *path)
{
    process_header *process = (process_header *)malloc(sizeof(process_header));
    process->page_directory = malloc_align(1024 * 4, 1024 * 4);

    paging_table_entry *page_directory = paging_get_page_directory();
    memcpy(process->page_directory, (void *)page_directory, 1024 * 4);

    paging_set_page_directory((void *)((uint32_t)process->page_directory - 0xC0000000));

    uint16_t sectors, size;
    uint8_t *content = fat12_read_file("/ENV/SHELL.ELF", &sectors, &size);

    elf_header *app_header = elf_get_header(content);
    enter_user_space(app_header->entry_position);
}