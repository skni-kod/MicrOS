#include "../fenv.h"

int fesetenv(const fenv_t *envp)
{
    if(envp == NULL)
    {
        return 1;
    }
    fenv_t current = __FPU_read_environment();
    
    //Copy all data without exceptions flags.    
    current.control_word = envp->control_word;
    current.data_pointer_offset = envp->data_pointer_offset;
    current.data_pointer_selector = envp->data_pointer_selector;
    current.instruction_pointer_offset = envp->instruction_pointer_offset;
    current.instruction_pointer_selector = envp->instruction_pointer_selector;
    current.opcode = envp->opcode;
    current.tag_word = envp->tag_word;
    current.status_word.stack_fault = envp->status_word.stack_fault;
    current.status_word.error_summary_status = envp->status_word.error_summary_status;
    current.status_word.condition_code_0 = envp->status_word.condition_code_0;
    current.status_word.condition_code_1 = envp->status_word.condition_code_1;
    current.status_word.condition_code_2 = envp->status_word.condition_code_2;
    current.status_word.top_of_stack_pointer = envp->status_word.top_of_stack_pointer;
    current.status_word.condition_code_3 = envp->status_word.condition_code_3;
    current.status_word.busy = envp->status_word.busy;
    __FPU_write_environment(current);
    return 0;
}