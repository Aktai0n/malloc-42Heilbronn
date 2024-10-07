
#include "alloc_block.h"

void add_to_alloc_list(t_alloc_block** list, t_alloc_block* new_block) {
    if (*list != NULL) {
        new_block->next = *list;
    }
    *list = new_block;
}

t_alloc_block* delete_from_alloc_list(t_alloc_block** list, t_alloc_block* to_remove) {
    t_alloc_block* prev = NULL;
    for (t_alloc_block* block = *list; block != NULL;) {
        if (block == to_remove) {
            if (block == *list) {
                *list = block->next;
            } else {
                prev->next = block->next;
            }
            block->next = NULL;
            return block;
        }
        prev = block;
        block = block->next;
    }
    return NULL;
}

t_alloc_block* find_alloc_block(t_alloc_block* list_head, const size_t size) {
    t_alloc_block* best_fit = NULL;
    for (
        t_alloc_block* block = list_head;
        block != NULL;
        block = block->next
    ) {
        size_t block_size = get_alloc_size(block);
        if (block_size == size) {
            return block;
        } else if (
            block_size > size &&
            (best_fit == NULL ||
            block_size - size < get_alloc_size(best_fit) - size)
        ) {
            best_fit = block;
        }
    }
    return best_fit;
}
