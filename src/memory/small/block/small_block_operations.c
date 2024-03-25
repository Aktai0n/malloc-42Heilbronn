
#include "small_block.h"

static void set_next_block_flag_(
    t_small_block* block,
    size_t flag,
    bool state
) {
    t_small_block* next = get_next_small_block(block);
    if (next != NULL) {
        set_block_flag(&next->prev, flag, state);
    }
}

t_small_block* allocate_small_block(
    const size_t size,
    t_small_block* list
) {
    t_small_block* block = find_small_block(list, size);
    if (block == NULL) {
        return NULL;
    }

    split_small_block(block, size);

    set_block_flag(&block->curr, IS_ALLOCATED_FLAG, true);
    set_next_block_flag_(block, IS_ALLOCATED_FLAG, true);
    return block;
}

bool deallocate_small_block(t_small_block* block) {
    merge_small_block(&block);
    set_block_flag(&block->curr, IS_ALLOCATED_FLAG, false);
    set_next_block_flag_(block, IS_ALLOCATED_FLAG, false);
}

