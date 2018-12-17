#include "process_manager.h"

vector processes;
uint32_t current_process_id = 0;
uint32_t next_process_id = 0;

extern void enter_user_space(interrupt_state address, uint32_t code_segment, uint32_t data_segment);

void process_manager_init()
{
    vector_init(&processes);
    idt_attach_interrupt_handler(0, process_manager_interrupt_handler);
}

uint32_t process_manager_create_process(char *path)
{
    process_header *process = (process_header *)heap_kernel_alloc(sizeof(process_header), 0);
    process->id = next_process_id++;
    process->page_directory = heap_kernel_alloc(1024 * 4, 1024 * 4);

    paging_table_entry *page_directory = paging_get_page_directory();
    memcpy(process->page_directory, (void *)page_directory, 1024 * 4);

    uint16_t sectors, size;
    uint8_t *content = fat12_read_file(path, &sectors, &size);

    elf_header *app_header = elf_get_header(content);
    uint32_t initial_page = elf_loader_load(content);

    process->base_heap_page_index = initial_page + 1;
    heap_set_user_heap((void *)(process->base_heap_page_index * 1024 * 1024 * 4));
    heap_init_user_heap();

    process->stack = heap_user_alloc(1024 * 1024, 0) + 1024 * 1024;

    process->state.eax = 0;
    process->state.ebp = 0;
    process->state.ecx = 0;
    process->state.edx = 0;
    process->state.esp = (uint32_t)process->stack;
    process->state.ebp = (uint32_t)process->stack;
    process->state.esi = 0;
    process->state.edi = 0;
    process->state.eip = app_header->entry_position;
    process->state.eflags = 0;

    if (processes.count == 0)
    {
        current_process_id = process->id;
    }

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

void process_manager_interrupt_handler(interrupt_state *state)
{
    //io_disable_interrupts();
    pic_confirm_master_and_slave();

    if (processes.count > 0)
    {
        uint32_t old_process_id = current_process_id;
        uint32_t new_process_id = (current_process_id + 1) % processes.count;

        process_header *old_process = processes.data[old_process_id];
        process_header *new_process = processes.data[new_process_id];

        __asm__("mov %0, %%eax\n"
                "mov %%eax, %%esp"
                :
                : "g"(new_process->stack)
                : "eax");

        enter_user_space(new_process->state, 0x1B, 0x23);
        old_process = 0;
    }

    //io_enable_interrupts();
}