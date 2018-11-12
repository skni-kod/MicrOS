#include "memory_manager.h"

uint32_t base_address;
object_header* first_object;

void memory_manager_init_memory(uint32_t initial_size)
{
    (*first_object).free = 1;
    (*first_object).size = initial_size;
    (*first_object).next = 0;
}

void memory_manager_set_base_address(uint32_t address)
{
    base_address = address;
    first_object = address;
}

void memory_manager_alloc()
{

}

void memory_manager_free()
{

}