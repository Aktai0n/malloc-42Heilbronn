

#include "alloc_block.h"

// finds a free block of memory to allocate through a best fit approach
t_alloc_block* find_alloc_block(t_alloc_block* free_list_head, const size_t size) {
    t_alloc_block* best_fit = NULL;
    for (
        t_alloc_block* block = free_list_head;
        block != NULL;
        block = block->next
    ) {
        if (block->size == size) {
            return block;
        }
        else if (
            block->size > size &&
            (best_fit == NULL ||
            block->size - size < best_fit->size - size)
        ) {
            best_fit = block;
        }
    }
    return best_fit;
}