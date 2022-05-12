#include "syscalls_manager.h"

void (*syscall_handlers[SYSCALLS_MANAGER_MAX_HANDLERS])(interrupt_state *state);

void syscalls_manager_init()
{
    idt_attach_syscalls_manager(syscalls_manager_call);

    // 0x00XX - Heap
    syscalls_manager_attach_handler(0x0000, syscall_heap_alloc_memory);
    syscalls_manager_attach_handler(0x0001, syscall_heap_realloc_memory);
    syscalls_manager_attach_handler(0x0002, syscall_heap_dealloc_memory);
    syscalls_manager_attach_handler(0x0003, syscall_heap_get_object_size);
    syscalls_manager_attach_handler(0x0004, syscall_heap_verify_integrity);
    syscalls_manager_attach_handler(0x0005, syscall_heap_get_process_heap);
    //HEAP DEBUG
    syscalls_manager_attach_handler(0x00FF, syscall_heap_kernel_alloc);
    syscalls_manager_attach_handler(0x00FE, syscall_heap_kernel_dealloc);

    // 0x01XX - VGA and generic VGA
    syscalls_manager_attach_handler(0x0100, syscall_terminal_print_char);
    syscalls_manager_attach_handler(0x0101, syscall_terminal_print_string);
    syscalls_manager_attach_handler(0x0102, syscall_terminal_get_char_at_position);
    syscalls_manager_attach_handler(0x0103, syscall_terminal_set_cursor_position);
    syscalls_manager_attach_handler(0x0104, syscall_terminal_get_cursor_position);
    syscalls_manager_attach_handler(0x0105, syscall_terminal_clear);
    syscalls_manager_attach_handler(0x0106, syscall_terminal_set_cursor_visibility);
    syscalls_manager_attach_handler(0x0107, syscall_videocard_set_video_mode);
    syscalls_manager_attach_handler(0x0108, syscall_videocard_is_text_mode);
    syscalls_manager_attach_handler(0x0109, syscall_videocard_get_current_video_mode);
    syscalls_manager_attach_handler(0x010A, syscall_videocard_is_vretrace);
    syscalls_manager_attach_handler(0x010B, syscall_terminal_set_video_mode);
    syscalls_manager_attach_handler(0x010C, syscall_terminal_copy_from_buffer);
    syscalls_manager_attach_handler(0x010D, syscall_videocard_find_video_mode);


    // 0x02XX - Keyboard
    syscalls_manager_attach_handler(0x0200, syscall_keyboard_is_key_pressed);
    syscalls_manager_attach_handler(0x0201, syscall_keyboard_get_pressed_key);
    syscalls_manager_attach_handler(0x0202, syscall_keyboard_wait_for_key_press);
    syscalls_manager_attach_handler(0x0203, syscall_keyboard_get_key_state);

    // 0x03XX - PCI

    // 0x04XX - RTC
    syscalls_manager_attach_handler(0x0400, syscall_rtc_get_time);
    syscalls_manager_attach_handler(0x0401, syscall_rtc_set_time);

    // 0x05XX - File system
    syscalls_manager_attach_handler(0x0500, syscall_filesystem_get_file_info);
    syscalls_manager_attach_handler(0x0501, syscall_filesystem_get_directory_info);
    syscalls_manager_attach_handler(0x0502, syscall_filesystem_read_file);
    syscalls_manager_attach_handler(0x0503, syscall_filesystem_get_entries_count_in_directory);
    syscalls_manager_attach_handler(0x0504, syscall_filesystem_get_entries_in_directory);
    syscalls_manager_attach_handler(0x0505, syscall_filesystem_is_file);
    syscalls_manager_attach_handler(0x0506, syscall_filesystem_is_directory);
    syscalls_manager_attach_handler(0x0507, syscall_filesystem_create_file);
    syscalls_manager_attach_handler(0x0508, syscall_filesystem_create_directory);
    syscalls_manager_attach_handler(0x0509, syscall_filesystem_delete_file);
    syscalls_manager_attach_handler(0x050A, syscall_filesystem_delete_directory);
    syscalls_manager_attach_handler(0x050B, syscall_filesystem_rename_file);
    syscalls_manager_attach_handler(0x050C, syscall_filesystem_rename_directory);
    syscalls_manager_attach_handler(0x050D, syscall_filesystem_save_to_file);
    syscalls_manager_attach_handler(0x050E, syscall_filesystem_append_to_file);
    syscalls_manager_attach_handler(0x050F, syscall_filesystem_get_free_space);
    syscalls_manager_attach_handler(0x0510, syscall_filesystem_get_total_space);

    // 0x06XX - Timer
    syscalls_manager_attach_handler(0x0600, syscall_timer_get_system_clock);

    // 0x07XX (OLD 0x9X) - Processes
    syscalls_manager_attach_handler(0x0700, syscall_process_exit);
    syscalls_manager_attach_handler(0x0701, syscall_process_get_processes_count);
    syscalls_manager_attach_handler(0x0702, syscall_process_get_current_process_info);
    syscalls_manager_attach_handler(0x0703, syscall_process_get_process_info);
    syscalls_manager_attach_handler(0x0704, syscall_process_get_all_processes_info);
    syscalls_manager_attach_handler(0x0705, syscall_process_set_current_process_name);
    syscalls_manager_attach_handler(0x0706, syscall_process_current_process_sleep);
    syscalls_manager_attach_handler(0x0707, syscall_process_start_process);
    syscalls_manager_attach_handler(0x0708, syscall_process_set_current_process_signal_handler);
    syscalls_manager_attach_handler(0x0709, syscall_process_finish_signal_handler);
    syscalls_manager_attach_handler(0x070A, syscall_process_wait_for_process);
    syscalls_manager_attach_handler(0x070B, syscall_process_start_thread);

    // 0x8X - PC Speaker
    syscalls_manager_attach_handler(0x0800, syscall_pc_speaker_enable_sound);
    syscalls_manager_attach_handler(0x0801, syscall_pc_speaker_disable_sound);

    //0x09XX RESERVED

    // 0xAX - Memory
    syscalls_manager_attach_handler(0x0A00, syscall_memory_get_physical_memory_stats);
    
    // 0xBX - Partitions
    syscalls_manager_attach_handler(0x0B00, syscall_partitions_get_count);
    syscalls_manager_attach_handler(0x0B01, syscall_partitions_get_symbols);
    syscalls_manager_attach_handler(0x0B02, syscall_partitions_get_info);
    
    // 0xCX - Power
    syscalls_manager_attach_handler(0x0C00, syscall_power_reboot);
    syscalls_manager_attach_handler(0x0C01, syscall_power_shutdown);
    
    // 0xDX - Serial
    syscalls_manager_attach_handler(0x0D00, syscall_serial_init);
    syscalls_manager_attach_handler(0x0D01, syscall_serial_is_busy);
    syscalls_manager_attach_handler(0x0D02, syscall_serial_is_queue_empty);
    syscalls_manager_attach_handler(0x0D03, syscall_serial_send);
    syscalls_manager_attach_handler(0x0D04, syscall_serial_send_string);
    syscalls_manager_attach_handler(0x0D05, syscall_serial_receive);
}

void syscalls_manager_attach_handler(uint16_t function_number, void (*handler)(interrupt_state *state))
{
    syscall_handlers[function_number] = handler;
}

void syscalls_manager_detach_handler(uint16_t function_number)
{
    syscall_handlers[function_number] = 0;
}

void syscalls_manager_call(interrupt_state *state)
{
    if (syscall_handlers[state->registers.eax] != 0)
    {
        (*syscall_handlers[state->registers.eax])(state);
    }
}