#include "process_manager.h"

volatile vector processes;
volatile uint32_t current_process_id = 0;
volatile uint32_t next_process_id = 0;
volatile uint32_t last_task_switch = 0;

extern void enter_user_space(interrupt_state *address);

void process_manager_init()
{
    last_task_switch = timer_get_system_clock();

    vector_init(&processes);
    idt_attach_interrupt_handler(0, process_manager_interrupt_handler, true);
}

uint32_t process_manager_create_process(char *path)
{
    process_header *process = (process_header *)heap_kernel_alloc(sizeof(process_header), 0);
    process->id = next_process_id++;
    process->page_directory = heap_kernel_alloc(1024 * 4, 1024 * 4);

    paging_table_entry *page_directory = paging_get_page_directory();
    memcpy(process->page_directory, (void *)page_directory, 1024 * 4);

    paging_set_page_directory(process->page_directory);

    uint16_t sectors, size;
    uint8_t *content = fat12_read_file(path, &sectors, &size);

    elf_header *app_header = elf_get_header(content);
    uint32_t initial_page = elf_loader_load(content);

    process->base_heap_page_index = initial_page + 1;
    heap_set_user_heap((void *)(process->base_heap_page_index * 1024 * 1024 * 4));
    heap_init_user_heap();

    process->stack = heap_user_alloc(1024 * 1024, 4) + 1024 * 1024;
    process->state.eip = app_header->entry_position;
    process->state.esp = (uint32_t)process->stack;
    process->state.interrupt_number = 0;
    process->state.eflags = 0x200;
    process->state.cs = 0x1B;
    process->state.ss = 0x23;
    process->initialized = false;

    process->state.registers.eax = 0;
    process->state.registers.ebx = 0;
    process->state.registers.ecx = 0;
    process->state.registers.edx = 0;
    process->state.registers.esp_unused = (uint32_t)process->stack;
    process->state.registers.ebp = 0;
    process->state.registers.esi = 0;
    process->state.registers.edi = 0;

    if (processes.count == 0)
    {
        current_process_id = process->id;
    }

    vector_add(&processes, process);
    heap_kernel_dealloc(content);

    paging_set_page_directory(page_directory);
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
    if (processes.count > 0 && timer_get_system_clock() - last_task_switch >= 100)
    {
        last_task_switch = timer_get_system_clock();

        uint32_t old_process_id = current_process_id;
        uint32_t new_process_id = (current_process_id + 1) % processes.count;

        process_header *old_process = processes.data[old_process_id];
        process_header *new_process = processes.data[new_process_id];

        current_process_id = new_process_id;
        if (new_process->initialized)
        {
            memcpy(&old_process->state, state, sizeof(interrupt_state));
        }
        else
        {
            new_process->initialized = true;
        }

        paging_set_page_directory(new_process->page_directory);
        heap_set_user_heap((void *)(new_process->base_heap_page_index * 1024 * 1024 * 4));

        pic_confirm_master_and_slave();
        __asm__("mov %0, %%eax\n"
                "mov %%eax, %%esp"
                :
                : "g"(new_process->state.esp)
                : "eax");

        enter_user_space(&new_process->state);
    }
}