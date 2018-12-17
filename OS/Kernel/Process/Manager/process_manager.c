#include "process_manager.h"

vector processes;
uint32_t current_process_id = 0;
uint32_t next_process_id = 0;

extern void enter_user_space(uint32_t address);

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

    if (processes.count == 0)
    {
        current_process_id = process->id;
    }

    vector_add(&processes, process);
    heap_kernel_dealloc(content);

    __asm__("mov %0, %%eax\n"
            "mov %%eax, %%esp"
            :
            : "g"(process->stack)
            : "eax");

    enter_user_space(app_header->entry_position);

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
    io_disable_interrupts();

    /*if (idt_get_interrupts_count() == 1 && processes.count > 0)
    {
        uint32_t old_process_id = current_process_id;
        uint32_t new_process_id = (current_process_id + 1) % processes.count;

        process_header *old_process = processes.data[old_process_id];
        process_header *new_process = processes.data[new_process_id];

        old_process->state.eax = state->eax;
        old_process->state.ebp = state->ebp;
        old_process->state.ecx = state->ecx;
        old_process->state.edx = state->edx;
        old_process->state.esp = state->esp;
        old_process->state.ebp = state->ebp;
        old_process->state.esi = state->esi;
        old_process->state.edi = state->edi;

        old_process->state.cs = state->cs;
        old_process->state.eip = state->eip;
        old_process->state.eflags = state->eflags;

        state->eax = new_process->state.eax;
        state->ebx = new_process->state.ebx;
        state->ecx = new_process->state.ecx;
        state->edx = new_process->state.edx;
        state->esp = new_process->state.esp;
        state->ebp = new_process->state.ebp;
        state->esi = new_process->state.esi;
        state->edi = new_process->state.edi;

        state->cs = new_process->state.cs;
        state->eip = new_process->state.eip;
        state->eflags = new_process->state.eflags;

        paging_set_page_directory((uint32_t)new_process->page_directory);
    }*/

    io_enable_interrupts();
}