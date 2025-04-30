//
// Created by kokolor on 30/04/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

struct block
{
    uint64_t size;
    struct block* next;
};

struct heap
{
    void* start;
    uint64_t size;
    struct block* free_list;
};

extern struct heap g_kernel_heap;

void* heap_alloc(struct heap* heap, uint64_t size);
void heap_free(struct heap* heap, void* pointer);
void heap_init(struct heap* heap, void* start, const uint64_t size);

#endif //HEAP_H
