
#include "alloc_block.h"

static bool can_be_merged_(t_alloc_block* block, t_alloc_block* next) {
    return next != NULL && !is_allocated(block) && !is_allocated(next);
}

bool merge_alloc_block(t_alloc_block* block, t_alloc_block** free_list) {
    t_alloc_block* next = get_next_block_in_memory(block);
    if (!can_be_merged_(block, next)) {
        return false;
    }
    delete_from_alloc_list(free_list, next);
    // increase the capacity of this block
    block->size = get_alloc_size(block) + get_alloc_size(next) + sizeof(*next);
    add_to_alloc_list(free_list, block);
    return true;
}