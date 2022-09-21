#include <klibrary/kqueue.h>

kqueue_t *kqueue_init(uint32_t blocks, size_t block_size)
{
    uint32_t req_blocks = blocks + blocks % 2;
    size_t mem = sizeof(kqueue_t) + req_blocks * block_size;
    kqueue_t *ret = heap_kernel_alloc(mem, 0);
    memset(ret, 0, mem);
    ret->size = req_blocks;
    ret->block_size = block_size;

    return ret;
}

size_t read(kqueue_t *queue, void *buffer)
{
    if(queue->read == queue->write)
        return 0;

    memcpy(buffer, (queue->data + (queue->read++ & queue->size - 1) * queue->block_size), queue->block_size);
    return queue->block_size;
}

size_t write(kqueue_t *queue, void *buffer)
{
    if(queue->write - queue->read == queue->size)
        return 0;
    memcpy((queue->data + (queue->write++ & queue->size - 1) * queue->block_size), buffer, queue->block_size);
    return queue->block_size;
}
