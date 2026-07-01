#include "heap.h"
#include "critical.h"
#include <stdint.h>
#include <stdbool.h>

extern uint32_t _heap_start;
extern uint32_t _heap_end;

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define POISON_ALLOC 0xCD
#define POISON_FREE  0xEF

typedef struct heap_block {
    size_t size;
    bool used;
    struct heap_block *next;
} heap_block_t;

static heap_block_t *head = NULL;

static void mem_poison(void *ptr, size_t size, uint8_t val) {
    uint8_t *p = (uint8_t *)ptr;
    for (size_t i = 0; i < size; i++) {
        p[i] = val;
    }
}

status_t heap_init(void)
{
    uint8_t *heap_start = (uint8_t *)&_heap_start;
    uint8_t *heap_end = (uint8_t *)&_heap_end;
    
    if (heap_start >= heap_end) {
        return STATUS_ERROR;
    }
    
    size_t total_size = (size_t)(heap_end - heap_start);
    if (total_size <= sizeof(heap_block_t)) {
        return STATUS_ERROR;
    }
    
    head = (heap_block_t *)heap_start;
    head->size = total_size - sizeof(heap_block_t);
    head->used = false;
    head->next = NULL;
    
    mem_poison((uint8_t *)head + sizeof(heap_block_t), head->size, POISON_FREE);
    
    return STATUS_OK;
}

void *heap_alloc(size_t size)
{
    if (size == 0 || !head) return NULL;
    
    size = ALIGN(size);
    
    critical_enter();
    
    heap_block_t *curr = head;
    while (curr) {
        if (!curr->used && curr->size >= size) {
            /* Found a suitable block, check if we can split it */
            if (curr->size >= size + sizeof(heap_block_t) + ALIGNMENT) {
                heap_block_t *new_block = (heap_block_t *)((uint8_t *)curr + sizeof(heap_block_t) + size);
                new_block->size = curr->size - size - sizeof(heap_block_t);
                new_block->used = false;
                new_block->next = curr->next;
                
                curr->size = size;
                curr->next = new_block;
            }
            
            curr->used = true;
            mem_poison((uint8_t *)curr + sizeof(heap_block_t), curr->size, POISON_ALLOC);
            
            critical_exit();
            return (void *)((uint8_t *)curr + sizeof(heap_block_t));
        }
        curr = curr->next;
    }
    
    critical_exit();
    return NULL;
}

static void coalesce_free_blocks(void)
{
    heap_block_t *curr = head;
    while (curr && curr->next) {
        if (!curr->used && !curr->next->used) {
            /* Merge with the next block */
            curr->size += sizeof(heap_block_t) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

status_t heap_free(void *ptr)
{
    if (!ptr || !head) return STATUS_INVALID_PARAM;
    
    critical_enter();
    
    heap_block_t *curr = head;
    heap_block_t *target = (heap_block_t *)((uint8_t *)ptr - sizeof(heap_block_t));
    
    /* Validate pointer exists in our list */
    bool found = false;
    while (curr) {
        if (curr == target) {
            found = true;
            break;
        }
        curr = curr->next;
    }
    
    if (found && target->used) {
        target->used = false;
        mem_poison((uint8_t *)target + sizeof(heap_block_t), target->size, POISON_FREE);
        coalesce_free_blocks();
        critical_exit();
        return STATUS_OK;
    }
    
    critical_exit();
    return STATUS_ERROR;
}

size_t heap_available(void)
{
    if (!head) return 0;
    
    critical_enter();
    size_t available = 0;
    heap_block_t *curr = head;
    
    while (curr) {
        if (!curr->used) {
            available += curr->size;
        }
        curr = curr->next;
    }
    critical_exit();
    
    return available;
}

size_t heap_used(void)
{
    if (!head) return 0;
    
    critical_enter();
    size_t used = 0;
    heap_block_t *curr = head;
    
    while (curr) {
        if (curr->used) {
            used += curr->size;
        }
        curr = curr->next;
    }
    critical_exit();
    
    return used;
}
