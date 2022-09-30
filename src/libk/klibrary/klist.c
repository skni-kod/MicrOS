#include "klist.h"

klist_t *klist_init()
{
    klist_t *data = 0;
    data = heap_kernel_alloc(sizeof(klist_t), 0);
    if (data)
    {
        data->value = NULL;
        data->next = NULL;
        data->prev = NULL;
        data->size = NULL;
        return data;
    }
    else
        return 0;
}

klist_t *klist_add(klist_t *head, void *value)
{
    if (head == NULL)
    {
        head = heap_kernel_alloc(sizeof(klist_t), 0);
        head->prev = NULL;
        head->value = value;
        head->next = NULL;
        head->size = 1;
        return head;
    }

    if (head->prev == NULL && head->next == NULL && head->value == NULL)
    {
        head->value = value;
        head->size = 1;
        return head;
    }

    // add to existing 
    klist_t *new = heap_kernel_alloc(sizeof(klist_t), 0);
    new->next = head;
    new->value = value;
    new->prev = NULL;
    head->prev = new;
    ++head->size;
    new->size = head->size;

    return new;
}

klist_t *klist_delete(klist_t *head, klist_t *ptr)
{
    if (head)
    {
        if (head != ptr)
        {
            if (ptr->prev && ptr->next)
            {
                ((klist_t*)(ptr->prev))->next = ptr->next;
                ((klist_t*)(ptr->next))->prev = ptr->prev;
                heap_kernel_dealloc(ptr);
                --head->size;
                return head;
            }
            if (!ptr->next && !ptr->prev)
            {
                heap_kernel_dealloc(ptr);
                return 0;
            }
            if (ptr->next && !ptr->prev)
            {
                ((klist_t*)(ptr->next))->prev = NULL;
                heap_kernel_dealloc(ptr);
                --ptr->next->size;
                return ptr->next;
            }
            if (!ptr->next && ptr->prev)
            {
                ((klist_t*)(ptr->prev))->next = NULL;
                heap_kernel_dealloc(ptr);
                --head->size;
                return head;
            }
        }
        else
        {
            if (head->next)
            {
                klist_t *tmp = (klist_t*)head->next;
                ((klist_t*)(head->next))->prev = NULL;
                heap_kernel_dealloc(head);
                --tmp->size;
                return tmp;
            }
            heap_kernel_dealloc(head);
            head = 0;
            ptr = 0;
            return 0;
        }
    }

    return 0;
}

klist_t *klist_clear(klist_t *head)
{
    klist_t *tmp = head;
    if (head != NULL)
    {
        while (1)
        {
            if (tmp->next == NULL)
            {
                heap_kernel_dealloc(tmp);
                return head;
            }
            tmp = tmp->next;
            heap_kernel_dealloc(tmp->prev);
        }
    }
    return 0;
}
