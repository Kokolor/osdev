//
// Created by kokolor on 30/04/25.
//

#include <stddef.h>
#include "heap.h"

#define ALIGN8(x) (((x) + 7) & ~7)

struct heap g_kernel_heap;

void* heap_alloc(struct heap* heap, uint64_t size)
{
    size = ALIGN8(size);
    struct block* previous_block = NULL;
    struct block* current_block = heap->free_list;

    while (current_block)
    {
        if (current_block->size >= size)
        {
            if (current_block->size >= size + sizeof(struct block) + 8)
            {
                struct block* new_block = (struct block*)((uint8_t*)current_block + sizeof(struct block) + size);
                new_block->size = current_block->size - size - sizeof(struct block);
                new_block->next = current_block->next;
                current_block->size = size;
                if (previous_block)
                {
                    previous_block->next = new_block;
                }
                else
                {
                    heap->free_list = new_block;
                }
            }
            else
            {
                if (previous_block)
                {
                    previous_block->next = current_block->next;
                }
                else
                {
                    heap->free_list = current_block->next;
                }
            }

            return (uint8_t*)current_block + sizeof(struct block);
        }

        previous_block = current_block;
        current_block = current_block->next;
    }

    return NULL;
}

void heap_free(struct heap* heap, void* pointer)
{
    if (!pointer)
        return;

    struct block* block = (struct block*)((uint8_t*)pointer - sizeof(struct block*));
    block->next = heap->free_list;
    heap->free_list = block;
    struct block* current_block = heap->free_list;

    while (current_block)
    {
        struct block* next_block = current_block->next;
        if (next_block && (uint8_t*)current_block + sizeof(struct block*) + current_block->size == (uint8_t*)next_block)
        {
            current_block->size += sizeof(struct block*) + next_block->size;
            current_block->next = next_block->next;
        }
        else
        {
            current_block = current_block->next;
        }
    }
}

void heap_init(struct heap* heap, void* start, const uint64_t size)
{
    heap->start = start;
    heap->size = size;
    heap->free_list = (struct block*)start;
    heap->free_list->size = size - sizeof(struct block);
    heap->free_list->next = NULL;
}
