#ifndef KLIST_H
#define KLIST_H

#include <stdint.h>
#include <memory/heap/heap.h>
#include <string.h>

typedef struct klist
{
	void *value;
	struct klist *next;
	struct klist *prev;
	int32_t size;
} klist_t;

klist_t *klist_init();
klist_t *klist_add(klist_t *head, void *value);
klist_t *klist_delete(klist_t *head, klist_t *ptr);
klist_t *klist_clear(klist_t *head);

#endif