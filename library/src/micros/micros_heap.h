#ifndef MICROS_HEAP_H
#define MICROS_HEAP_H

#include <stdbool.h>
#include "micros_interrupts.h"

//! Structure containing information about application heap and its entries
typedef struct micros_heap_entry
{
    //! Size of the current entry
    uint32_t size;
    
    //! Flag indicates if the entry represents some allocated object or not
    uint8_t free;

    //! Pointer to the next entry (0 if last)
    struct micros_heap_entry *next;
    
    //! Pointer to the previous entry (0 if first)
    struct micros_heap_entry *prev;
} __attribute__((packed)) micros_heap_entry;

#ifdef __cplusplus
extern "C" {
#endif

//! Allocates chunk of memory with the specified size and alignment
/*!
    Allocates chunk of memory with the specified size and alignment. Then returns pointer if success or 0 if there is not enough memory.
    \param size Size of the requested chunk of memory.
    \param align Alignment of the requested chunk of memory.
    \return Pointer if success of 0 in there is not enough memory.
*/
void *micros_heap_alloc(uint32_t size, uint32_t align);

//! Reallocates chunk of memory with the specified size
/*!
    Reallocates chunk of memory with the specified size. Then returns pointer if success or 0 if there is not enough memory.
    \param ptr Pointer to the existing heap entry.
    \param size Size of the requested chunk of memory.
    \return Pointer if success of 0 in there is not enough memory.
*/
void *micros_heap_realloc(void *ptr, uint32_t size);

//! Deallocates chunk of memory
/*!
    Deallocates chunk of memory passed in the parameter.
    \param ptr Pointer to the existing heap entry.
*/
void micros_heap_dealloc(void *ptr);

//! Retrieves size of the heap object
/*!
    Retrieves size of the heap object passed in the parameter.
    \param ptr Pointer to the existing heap entry.
    \return Size of the heap object.
*/
uint32_t micros_heap_get_object_size(void *ptr);

//! Verifies heap integrity
/*!
    Verifies heap integrity (objects aren't overlapping and sizes are correctly set) and returns boolean with result.
    \return True if heap is in the valid state, otherwise false.
*/
bool micros_heap_verify_integrity();

//! Retrieves application heap
/*!
    Retrieves application heap by returning first first entry.
    \return Pointer to the first entry of the application heap.
*/
micros_heap_entry *micros_heap_get_process_heap();

#ifdef __cplusplus
}
#endif

#endif