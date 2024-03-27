
#include "small_block.h"
#include "defines.h"


static t_small_block* merge_with_next_(
    t_small_block* block,
    t_small_block* next
) {
    // increase the capacity of block
    size_t block_size = get_block_size(block->curr);
    size_t next_size = get_block_size(next->curr);
    size_t new_size = block_size + new_size + sizeof(*next);
    set_block_size(&block->curr, new_size);
    
    if (is_last_block(next->curr)) {
        set_block_flag(&block->curr, IS_LAST_BLOCK_FLAG, true);
    } else {
        // set prev of the block after next
        // to the current block
        t_small_block* next_next = get_next_small_block(next);
        next_next->prev = block->curr;
    }

    // destroy next
    next->curr.size = 0;
    next->prev.size = 0;
    return block;
}

bool merge_small_block(t_small_block** block, bool merge_backwards) {
    bool ret = false;
    t_small_block* next = get_next_small_block(*block);
    while (next != NULL && !is_allocated(next->curr)) {
        ret = true;
        merge_with_next_(*block, next);
        next = get_next_small_block(*block);
    }

    if (merge_backwards) {
        t_small_block* prev = get_prev_small_block(*block);
        while (prev != NULL && !is_allocated(prev->curr)) {
            ret = true;
            *block = merge_with_next_(prev, *block);
            prev = get_prev_small_block(*block);
        }
    }
    return ret;
}

static bool can_be_splitted_(
    const size_t block_size,
    const size_t split_size
) {
    return block_size >=
        split_size + FT_MALLOC_ALIGNMENT + sizeof(t_small_block);
}

bool split_small_block(t_small_block* block, const size_t split_size) {
    const size_t block_size = get_block_size(block->curr);
    if (!can_be_splitted_(block, split_size)) {
        return false;
    }

    // create and initialize new block
    t_small_block* new_block = (t_small_block*)(
        (size_t)get_small_block_data(block) + split_size
    );
    size_t new_block_size = block_size - (split_size + sizeof(*block));
    set_block_size(&new_block->curr, new_block_size);

    // transfer flags
    set_block_flag(&new_block->curr, IS_ALLOCATED_FLAG, false);
    if (is_last_block(block->curr)) {
        set_block_flag(&new_block->curr, IS_LAST_BLOCK_FLAG, true);
        set_block_flag(&block->curr, IS_LAST_BLOCK_FLAG, false);
    } else {
        set_block_flag(&new_block->curr, IS_LAST_BLOCK_FLAG, false);
    }

    // shrink original block
    set_block_size(&block->curr, split_size);
    new_block->prev = block->curr;
    return true;
}
